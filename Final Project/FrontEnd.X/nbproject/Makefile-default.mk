#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=SchedMain.c keypadDriver.c rs232.c startup.c LCD.c rfidReader.c motorDriver.c game.c SRAMfront.c i2cComm.c LED.c xbee.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/SchedMain.o ${OBJECTDIR}/keypadDriver.o ${OBJECTDIR}/rs232.o ${OBJECTDIR}/startup.o ${OBJECTDIR}/LCD.o ${OBJECTDIR}/rfidReader.o ${OBJECTDIR}/motorDriver.o ${OBJECTDIR}/game.o ${OBJECTDIR}/SRAMfront.o ${OBJECTDIR}/i2cComm.o ${OBJECTDIR}/LED.o ${OBJECTDIR}/xbee.o
POSSIBLE_DEPFILES=${OBJECTDIR}/SchedMain.o.d ${OBJECTDIR}/keypadDriver.o.d ${OBJECTDIR}/rs232.o.d ${OBJECTDIR}/startup.o.d ${OBJECTDIR}/LCD.o.d ${OBJECTDIR}/rfidReader.o.d ${OBJECTDIR}/motorDriver.o.d ${OBJECTDIR}/game.o.d ${OBJECTDIR}/SRAMfront.o.d ${OBJECTDIR}/i2cComm.o.d ${OBJECTDIR}/LED.o.d ${OBJECTDIR}/xbee.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/SchedMain.o ${OBJECTDIR}/keypadDriver.o ${OBJECTDIR}/rs232.o ${OBJECTDIR}/startup.o ${OBJECTDIR}/LCD.o ${OBJECTDIR}/rfidReader.o ${OBJECTDIR}/motorDriver.o ${OBJECTDIR}/game.o ${OBJECTDIR}/SRAMfront.o ${OBJECTDIR}/i2cComm.o ${OBJECTDIR}/LED.o ${OBJECTDIR}/xbee.o

# Source Files
SOURCEFILES=SchedMain.c keypadDriver.c rs232.c startup.c LCD.c rfidReader.c motorDriver.c game.c SRAMfront.c i2cComm.c LED.c xbee.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46K22
MP_PROCESSOR_OPTION_LD=18f46k22
MP_LINKER_DEBUG_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/SchedMain.o: SchedMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SchedMain.o.d 
	@${RM} ${OBJECTDIR}/SchedMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SchedMain.o   SchedMain.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SchedMain.o 
	@${FIXDEPS} "${OBJECTDIR}/SchedMain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/keypadDriver.o: keypadDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/keypadDriver.o.d 
	@${RM} ${OBJECTDIR}/keypadDriver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/keypadDriver.o   keypadDriver.c 
	@${DEP_GEN} -d ${OBJECTDIR}/keypadDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/keypadDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/rs232.o: rs232.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/rs232.o.d 
	@${RM} ${OBJECTDIR}/rs232.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/rs232.o   rs232.c 
	@${DEP_GEN} -d ${OBJECTDIR}/rs232.o 
	@${FIXDEPS} "${OBJECTDIR}/rs232.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/startup.o: startup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/startup.o.d 
	@${RM} ${OBJECTDIR}/startup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/startup.o   startup.c 
	@${DEP_GEN} -d ${OBJECTDIR}/startup.o 
	@${FIXDEPS} "${OBJECTDIR}/startup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/LCD.o: LCD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/LCD.o.d 
	@${RM} ${OBJECTDIR}/LCD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/LCD.o   LCD.c 
	@${DEP_GEN} -d ${OBJECTDIR}/LCD.o 
	@${FIXDEPS} "${OBJECTDIR}/LCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/rfidReader.o: rfidReader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/rfidReader.o.d 
	@${RM} ${OBJECTDIR}/rfidReader.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/rfidReader.o   rfidReader.c 
	@${DEP_GEN} -d ${OBJECTDIR}/rfidReader.o 
	@${FIXDEPS} "${OBJECTDIR}/rfidReader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/motorDriver.o: motorDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/motorDriver.o.d 
	@${RM} ${OBJECTDIR}/motorDriver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/motorDriver.o   motorDriver.c 
	@${DEP_GEN} -d ${OBJECTDIR}/motorDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/motorDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/game.o: game.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/game.o.d 
	@${RM} ${OBJECTDIR}/game.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/game.o   game.c 
	@${DEP_GEN} -d ${OBJECTDIR}/game.o 
	@${FIXDEPS} "${OBJECTDIR}/game.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SRAMfront.o: SRAMfront.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SRAMfront.o.d 
	@${RM} ${OBJECTDIR}/SRAMfront.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SRAMfront.o   SRAMfront.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SRAMfront.o 
	@${FIXDEPS} "${OBJECTDIR}/SRAMfront.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/i2cComm.o: i2cComm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2cComm.o.d 
	@${RM} ${OBJECTDIR}/i2cComm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/i2cComm.o   i2cComm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/i2cComm.o 
	@${FIXDEPS} "${OBJECTDIR}/i2cComm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/LED.o: LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/LED.o.d 
	@${RM} ${OBJECTDIR}/LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/LED.o   LED.c 
	@${DEP_GEN} -d ${OBJECTDIR}/LED.o 
	@${FIXDEPS} "${OBJECTDIR}/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/xbee.o: xbee.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/xbee.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/xbee.o   xbee.c 
	@${DEP_GEN} -d ${OBJECTDIR}/xbee.o 
	@${FIXDEPS} "${OBJECTDIR}/xbee.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/SchedMain.o: SchedMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SchedMain.o.d 
	@${RM} ${OBJECTDIR}/SchedMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SchedMain.o   SchedMain.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SchedMain.o 
	@${FIXDEPS} "${OBJECTDIR}/SchedMain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/keypadDriver.o: keypadDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/keypadDriver.o.d 
	@${RM} ${OBJECTDIR}/keypadDriver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/keypadDriver.o   keypadDriver.c 
	@${DEP_GEN} -d ${OBJECTDIR}/keypadDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/keypadDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/rs232.o: rs232.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/rs232.o.d 
	@${RM} ${OBJECTDIR}/rs232.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/rs232.o   rs232.c 
	@${DEP_GEN} -d ${OBJECTDIR}/rs232.o 
	@${FIXDEPS} "${OBJECTDIR}/rs232.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/startup.o: startup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/startup.o.d 
	@${RM} ${OBJECTDIR}/startup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/startup.o   startup.c 
	@${DEP_GEN} -d ${OBJECTDIR}/startup.o 
	@${FIXDEPS} "${OBJECTDIR}/startup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/LCD.o: LCD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/LCD.o.d 
	@${RM} ${OBJECTDIR}/LCD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/LCD.o   LCD.c 
	@${DEP_GEN} -d ${OBJECTDIR}/LCD.o 
	@${FIXDEPS} "${OBJECTDIR}/LCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/rfidReader.o: rfidReader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/rfidReader.o.d 
	@${RM} ${OBJECTDIR}/rfidReader.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/rfidReader.o   rfidReader.c 
	@${DEP_GEN} -d ${OBJECTDIR}/rfidReader.o 
	@${FIXDEPS} "${OBJECTDIR}/rfidReader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/motorDriver.o: motorDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/motorDriver.o.d 
	@${RM} ${OBJECTDIR}/motorDriver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/motorDriver.o   motorDriver.c 
	@${DEP_GEN} -d ${OBJECTDIR}/motorDriver.o 
	@${FIXDEPS} "${OBJECTDIR}/motorDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/game.o: game.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/game.o.d 
	@${RM} ${OBJECTDIR}/game.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/game.o   game.c 
	@${DEP_GEN} -d ${OBJECTDIR}/game.o 
	@${FIXDEPS} "${OBJECTDIR}/game.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SRAMfront.o: SRAMfront.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SRAMfront.o.d 
	@${RM} ${OBJECTDIR}/SRAMfront.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SRAMfront.o   SRAMfront.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SRAMfront.o 
	@${FIXDEPS} "${OBJECTDIR}/SRAMfront.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/i2cComm.o: i2cComm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2cComm.o.d 
	@${RM} ${OBJECTDIR}/i2cComm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/i2cComm.o   i2cComm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/i2cComm.o 
	@${FIXDEPS} "${OBJECTDIR}/i2cComm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/LED.o: LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/LED.o.d 
	@${RM} ${OBJECTDIR}/LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/LED.o   LED.c 
	@${DEP_GEN} -d ${OBJECTDIR}/LED.o 
	@${FIXDEPS} "${OBJECTDIR}/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/xbee.o: xbee.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/xbee.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Program Files (x86)/Microchip/mplabc18/v3.46/h" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/xbee.o   xbee.c 
	@${DEP_GEN} -d ${OBJECTDIR}/xbee.o 
	@${FIXDEPS} "${OBJECTDIR}/xbee.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    18f46k22_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "18f46k22_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   18f46k22_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "18f46k22_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/FrontEnd.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
