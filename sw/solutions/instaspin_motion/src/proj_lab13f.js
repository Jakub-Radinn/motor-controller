expRemoveAll();
expAdd ("gMotorVars[0].Flag_enableSys", getDecimal());
expAdd ("gMotorVars[0].Flag_Run_Identify", getDecimal());
expAdd ("gMotorVars[0].Flag_enableOffsetcalc", getDecimal());
expAdd ("gMotorVars[0].Flag_enableAlignment", getDecimal());
expAdd ("gMotorVars[0].RunPositionProfile", getDecimal());
expAdd ("gMotorVars[0].PosStepInt_MRev");
expAdd ("gMotorVars[0].PosStepFrac_MRev", getQValue(24));
expAdd ("gMotorVars[0].MaxVel_krpm", getQValue(24));
expAdd ("gMotorVars[0].MaxAccel_krpmps", getQValue(24));
expAdd ("gMotorVars[0].MaxDecel_krpmps", getQValue(24));
expAdd ("gMotorVars[0].MaxJrk_krpmps2", getQValue(20));
expAdd ("gMotorVars[0].SpinTAC.PosCtlStatus");
expAdd ("gMotorVars[0].SpinTAC.PosCtlBw_radps", getQValue(20));
expAdd ("gMotorVars[0].SpinTAC.PosCtlErrorID");
expAdd ("gMotorVars[0].SpinTAC.PosMoveStatus");
expAdd ("gMotorVars[0].SpinTAC.PosMoveCurveType");
expAdd ("gMotorVars[0].SpinTAC.PosMoveTime_ticks");
expAdd ("gMotorVars[0].SpinTAC.PosMoveTime_mticks");
expAdd ("gMotorVars[0].SpinTAC.PosMoveErrorID");

expAdd ("gMotorVars[0].VdcBus_kV", getQValue(24));
expAdd ("pid[0]", getQValue(24));
expAdd ("gPwmData[0]", getQValue(24));
expAdd ("gDrvSpi8301Vars[0]");

expAdd ("gMotorVars[1].Flag_Run_Identify", getDecimal());
expAdd ("gMotorVars[1].Flag_enableOffsetcalc", getDecimal());
expAdd ("gMotorVars[1].Flag_enableAlignment", getDecimal());
expAdd ("gMotorVars[1].RunPositionProfile", getDecimal());
expAdd ("gMotorVars[1].PosStepInt_MRev");
expAdd ("gMotorVars[1].PosStepFrac_MRev", getQValue(24));
expAdd ("gMotorVars[1].MaxVel_krpm", getQValue(24));
expAdd ("gMotorVars[1].MaxAccel_krpmps", getQValue(24));
expAdd ("gMotorVars[1].MaxDecel_krpmps", getQValue(24));
expAdd ("gMotorVars[1].MaxJrk_krpmps2", getQValue(20));
expAdd ("gMotorVars[1].SpinTAC.PosCtlStatus");
expAdd ("gMotorVars[1].SpinTAC.PosCtlBw_radps", getQValue(20));
expAdd ("gMotorVars[1].SpinTAC.PosCtlErrorID");
expAdd ("gMotorVars[1].SpinTAC.PosMoveStatus");
expAdd ("gMotorVars[1].SpinTAC.PosMoveCurveType");
expAdd ("gMotorVars[1].SpinTAC.PosMoveTime_ticks");
expAdd ("gMotorVars[1].SpinTAC.PosMoveTime_mticks");
expAdd ("gMotorVars[1].SpinTAC.PosMoveErrorID");

expAdd ("gMotorVars[1].VdcBus_kV", getQValue(24));
expAdd ("pid[1]", getQValue(24));
expAdd ("gPwmData[1]", getQValue(24));
expAdd ("gDrvSpi8301Vars[1]");