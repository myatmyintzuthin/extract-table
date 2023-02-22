
#
# CV Library Path
#
CV_LIB_PATH = '''%{cv_lib_path}'''

#
# RPATH Command Template
#
RPATH_COMMAND = '''
%{commands}
'''

#-------------------------------------------------------------------#
# For Linux                                                         #
#-------------------------------------------------------------------#

#
# OpenCV deploy process for linux
#
def _cv_deploy_linux( ctx ):
    
    in_file = ctx.file.src
    deploy_sh = ctx.actions.declare_file( "cv_deploy.sh" )

    copy_cmd = "cp -p %s/lib/libopencv_*.so %s" % (
        CV_LIB_PATH, in_file.dirname
    )

    ctx.actions.run_shell(
        inputs  = [ in_file ],
        outputs = [ deploy_sh ],
        command = "echo \"%s\" > %s;" %
                  ( copy_cmd, deploy_sh.path )
    )

    return [
        DefaultInfo( files = depset([ deploy_sh ]) )
    ]

#-------------------------------------------------------------------#
# Main Routine                                                      #
#-------------------------------------------------------------------#

#
# cv deploy decralation
#
cv_deploy = rule(
    implementation = %{function},
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = True,
        )
    }
)
