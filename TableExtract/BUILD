load( "@cv//:cv_deploy.bzl" , "cv_deploy" )
load( "@rules_cc//cc:defs.bzl" , "cc_library" )
load( "@rules_cc//cc:defs.bzl" , "cc_binary" )



BIN_NAME = "extract-table"

cc_library(
    name = "table" ,
    hdrs = ["ExtractTable.h"],
    srcs = ["ExtractTable.cpp"],
    copts = ["-std=c++17"],
    deps = [
        "@cv//:cv2",
        "@nlohmann_json//:json"
    ]

)

cc_binary(
    name = BIN_NAME,
    srcs = ["main.cpp"],
    copts = ["-std=c++17"],
    deps = [
        ":table" ,
        "@cv//:cv2" ,
        "@nlohmann_json//:json"
    ]

)

cv_deploy( name="cv_deploy", src=BIN_NAME )

genrule(
    name = "run" ,
    srcs = [ "cv_deploy" ],
    outs = [ "run.sh" ],
    cmd  = "echo $(SRCS) | sed \'s/ /\\n/g\' > $@",
    executable = 1,
)

genrule(
    name = "exec" ,
    srcs = [ "cv_deploy" ],
    outs = [ "run.bat" ],
    cmd  = "echo \" $(SRCS)\" | sed \'s/ /\\ncall /g\' > $@",
    executable = 1,
)
