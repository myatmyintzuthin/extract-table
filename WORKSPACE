#------------------------------------------------------------------------------
# Workspace name of root
#------------------------------------------------------------------------------
workspace( name="tcontour" )

#------------------------------------------------------------------------------
# OpenCV
#------------------------------------------------------------------------------
load( "@tcontour//External/OpenCV:cv_configure.bzl", "cv_autoconf" )
cv_autoconf( name = "cv" )

#------------------------------------------------------------------------------
# Json 
#------------------------------------------------------------------------------
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "nlohmann_json",
    build_file = "@tcontour//External/json:BUILD",
    sha256 = "4cf0df69731494668bdd6460ed8cb269b68de9c19ad8c27abc24cd72605b2d5b",
    strip_prefix = "json-3.9.1",
    urls = ["https://github.com/nlohmann/json/archive/v3.9.1.tar.gz"],
)