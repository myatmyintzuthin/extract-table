
#
# CV Framework List
#
CV_FRAMEWORK_LIST = [
    "calib3d",
    "core",
    "dnn",
    "features2d",
    "flann",
    "gapi",
    "highgui",
    "imgcodecs",
    "imgproc",
    "ml",
    "objdetect",
    "photo",
    "stitching",
    "video",
    "videoio",
]

#-------------------------------------------------------------------#
# For Linux                                                         #
#-------------------------------------------------------------------#

#
# CV Framework Format for Linux
#
CV_LIBRARY_FORMAT_FOR_LINUX = '''
cc_library(
    name = "cv2",
    srcs = [
        %s
    ],
    hdrs = glob( ["Headers/opencv4/**"] ),
    includes = [ "Headers/opencv4" ],
    visibility = [ "//visibility:public" ],
)
'''

#
# Generate Deploy Configure for Linux
#
def _gen_deploy_configure_linux( repository_ctx, lib_path ):
    
    tpl_path = repository_ctx.path(
        Label( "//External/OpenCV:cv_deploy.bzl.tpl" )
    )

    repository_ctx.template(
        "cv_deploy.bzl",
        tpl_path,
        {
            "%{cv_lib_path}": lib_path,
            "%{commands}": "",
            "%{function}": "_cv_deploy_linux",
        },
    )

#
# CV Repository Auto Configuration for Linux
#
def _cv_autoconf_for_linux( repository_ctx, lib_path, lib_version ):

    _gen_deploy_configure_linux( repository_ctx, lib_path )

    hdr_src_path = lib_path + "/include"
    hdr_dst_path = "./Headers"
    repository_ctx.symlink( hdr_src_path, hdr_dst_path )

    lib_src_path = lib_path + "/lib"
    lib_dst_path = "./Lib"
    repository_ctx.symlink( lib_src_path, lib_dst_path )

    lib_list = []
    for lib_name in CV_FRAMEWORK_LIST:
        lib_name = "\"Lib/libopencv_%s.so\"" % ( lib_name )
        lib_list.append( lib_name )

    contents = CV_LIBRARY_FORMAT_FOR_LINUX % ( ",\n        ".join( lib_list ) )
    repository_ctx.file( "BUILD", contents )


#-------------------------------------------------------------------#
# Main Routine                                                      #
#-------------------------------------------------------------------#

#
# CV Repository Auto Configuration
#
def _cv_autoconf_impl( repository_ctx ):
    
    os_name = repository_ctx.os.name.lower()
    lib_path = repository_ctx.os.environ[ "CV_LIB_PATH" ]
    lib_path = lib_path.replace( "\\", "/" )
    lib_version = repository_ctx.os.environ[ "CV_LIB_VERSION" ]
    if os_name.find( "linux" ) != -1:
        _cv_autoconf_for_linux( repository_ctx, lib_path, lib_version )

#
# CV Repository Rule Decralation
#
cv_autoconf = repository_rule(
    implementation = _cv_autoconf_impl,
    local = True,
    configure = True,
)