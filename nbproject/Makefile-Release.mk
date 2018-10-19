#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=clang
CXX=clang
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ChannelConvolutionPdf.o \
	${OBJECTDIR}/Constants.o \
	${OBJECTDIR}/ConvPdf.o \
	${OBJECTDIR}/ExpPdf.o \
	${OBJECTDIR}/FileUtils.o \
	${OBJECTDIR}/GrainPdf.o \
	${OBJECTDIR}/MyConvPdf.o \
	${OBJECTDIR}/MyPdf.o \
	${OBJECTDIR}/MyPdfCache.o \
	${OBJECTDIR}/ParamStorage.o \
	${OBJECTDIR}/StructParams.o \
	${OBJECTDIR}/ThreeExpPdf.o \
	${OBJECTDIR}/ThreeGaussian.o \
	${OBJECTDIR}/TrapPdf.o \
	${OBJECTDIR}/TwoExpPdf.o \
	${OBJECTDIR}/TwoGaussian.o \
	${OBJECTDIR}/positronfit.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread -stdlib=libc++ -std=c++11 -m64
CXXFLAGS=-pthread -stdlib=libc++ -std=c++11 -m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/Applications/root_v6.14.02/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/positronfit

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/positronfit: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	clang -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/positronfit ${OBJECTFILES} ${LDLIBSOPTIONS} -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl -lRooFit -lRooFitCore -lc++

${OBJECTDIR}/ChannelConvolutionPdf.o: ChannelConvolutionPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ChannelConvolutionPdf.o ChannelConvolutionPdf.cpp

${OBJECTDIR}/Constants.o: Constants.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Constants.o Constants.cpp

${OBJECTDIR}/ConvPdf.o: ConvPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ConvPdf.o ConvPdf.cpp

${OBJECTDIR}/ExpPdf.o: ExpPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExpPdf.o ExpPdf.cpp

${OBJECTDIR}/FileUtils.o: FileUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileUtils.o FileUtils.cpp

${OBJECTDIR}/GrainPdf.o: GrainPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GrainPdf.o GrainPdf.cpp

${OBJECTDIR}/MyConvPdf.o: MyConvPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MyConvPdf.o MyConvPdf.cpp

${OBJECTDIR}/MyPdf.o: MyPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MyPdf.o MyPdf.cpp

${OBJECTDIR}/MyPdfCache.o: MyPdfCache.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MyPdfCache.o MyPdfCache.cpp

${OBJECTDIR}/ParamStorage.o: ParamStorage.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ParamStorage.o ParamStorage.cpp

${OBJECTDIR}/StructParams.o: StructParams.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StructParams.o StructParams.cpp

${OBJECTDIR}/ThreeExpPdf.o: ThreeExpPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ThreeExpPdf.o ThreeExpPdf.cpp

${OBJECTDIR}/ThreeGaussian.o: ThreeGaussian.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ThreeGaussian.o ThreeGaussian.cpp

${OBJECTDIR}/TrapPdf.o: TrapPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TrapPdf.o TrapPdf.cpp

${OBJECTDIR}/TwoExpPdf.o: TwoExpPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TwoExpPdf.o TwoExpPdf.cpp

${OBJECTDIR}/TwoGaussian.o: TwoGaussian.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TwoGaussian.o TwoGaussian.cpp

${OBJECTDIR}/positronfit.o: positronfit.cc
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/Applications/root_v6.14.02/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/positronfit.o positronfit.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
