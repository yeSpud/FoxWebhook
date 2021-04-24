# Download cpr
ExternalProject_Add(
		cpr
		PREFIX "vendor/cpr"
		GIT_REPOSITORY "https://github.com/whoshuu/cpr.git"
		GIT_TAG origin/master
		TIMEOUT 10
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		UPDATE_COMMAND ""
)

# Prepare cURLpp (headers are located in the include directory)
ExternalProject_Get_Property(cpr source_dir)
set(CPR_INCLUDE_DIR ${source_dir}/include)