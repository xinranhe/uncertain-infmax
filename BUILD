cc_library(
	name = "uncertainty_infmax",
	srcs = [
		"Graph.cpp",
		"InfMaximizer.cpp",
		"UncertainInfExp.cpp",
		"UtilFunc.cpp",
	],
	hdrs = [
		"Graph.h",
		"InfMaximizer.h",
		"UncertainInfExp.h",
		"UtilFunc.h",
	],
	visibility = ["//visibility:public"],
)

cc_binary(
	name = "uncertainty_infmax_exp",
	srcs = ["main.cpp"],
	deps = ["uncertainty_infmax"],
	linkopts = [
		"-lm",
		"-fopenmp",
	],
	copts = [
		"-fopenmp",
	],
)
