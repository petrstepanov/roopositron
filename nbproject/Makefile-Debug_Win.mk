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
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_Win
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Constants.o \
	${OBJECTDIR}/ExpPdf.o \
	${OBJECTDIR}/FileUtils.o \
	${OBJECTDIR}/MyPdf.o \
	${OBJECTDIR}/MyPdfCache.o \
	${OBJECTDIR}/ParamStorage.o \
	${OBJECTDIR}/StructParams.o \
	${OBJECTDIR}/TwoExpPdf.o \
	${OBJECTDIR}/TwoGaussian.o \
	${OBJECTDIR}/positronfit.o


# C Compiler Flags
CFLAGS=-pthread

# CC Compiler Flags
CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/cygdrive/C/root_v5.34.36/lib -lm

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/positronfit.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/positronfit.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/positronfit ${OBJECTFILES} ${LDLIBSOPTIONS} -I/cygdrive/C/root_v5.34.36/include -pthread -std=c++11 -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread

${OBJECTDIR}/Constants.o: Constants.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Constants.o Constants.cpp

${OBJECTDIR}/ExpPdf.o: ExpPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExpPdf.o ExpPdf.cpp

${OBJECTDIR}/FileUtils.o: FileUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileUtils.o FileUtils.cpp

${OBJECTDIR}/MyPdf.o: MyPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MyPdf.o MyPdf.cpp

${OBJECTDIR}/MyPdfCache.o: MyPdfCache.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MyPdfCache.o MyPdfCache.cpp

${OBJECTDIR}/ParamStorage.o: ParamStorage.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ParamStorage.o ParamStorage.cpp

${OBJECTDIR}/StructParams.o: StructParams.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StructParams.o StructParams.cpp

${OBJECTDIR}/TwoExpPdf.o: TwoExpPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TwoExpPdf.o TwoExpPdf.cpp

${OBJECTDIR}/TwoGaussian.o: TwoGaussian.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TwoGaussian.o TwoGaussian.cpp

${OBJECTDIR}/positronfit.o: positronfit.cc
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/root_v5.34.36/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/positronfit.o positronfit.cc

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
