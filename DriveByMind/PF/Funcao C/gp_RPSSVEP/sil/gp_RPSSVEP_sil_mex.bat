@echo off
set MATLAB=C:\PROGRA~1\MATLAB\R2015a
set MATLAB_ARCH=win64
set MATLAB_BIN="C:\Program Files\MATLAB\R2015a\bin"
set ENTRYPOINT=mexFunction
set OUTDIR=.\
set LIB_NAME=gp_RPSSVEP_sil
set MEX_NAME=gp_RPSSVEP_sil
set MEX_EXT=.mexw64
call setEnv.bat
echo # Make settings for gp_RPSSVEP_sil > gp_RPSSVEP_sil_mex.mki
echo COMPILER=%COMPILER%>> gp_RPSSVEP_sil_mex.mki
echo COMPFLAGS=%COMPFLAGS%>> gp_RPSSVEP_sil_mex.mki
echo OPTIMFLAGS=%OPTIMFLAGS%>> gp_RPSSVEP_sil_mex.mki
echo DEBUGFLAGS=%DEBUGFLAGS%>> gp_RPSSVEP_sil_mex.mki
echo LINKER=%LINKER%>> gp_RPSSVEP_sil_mex.mki
echo LINKFLAGS=%LINKFLAGS%>> gp_RPSSVEP_sil_mex.mki
echo LINKOPTIMFLAGS=%LINKOPTIMFLAGS%>> gp_RPSSVEP_sil_mex.mki
echo LINKDEBUGFLAGS=%LINKDEBUGFLAGS%>> gp_RPSSVEP_sil_mex.mki
echo MATLAB_ARCH=%MATLAB_ARCH%>> gp_RPSSVEP_sil_mex.mki
echo BORLAND=%BORLAND%>> gp_RPSSVEP_sil_mex.mki
echo OMPFLAGS= >> gp_RPSSVEP_sil_mex.mki
echo OMPLINKFLAGS= >> gp_RPSSVEP_sil_mex.mki
echo EMC_COMPILER=msvc100>> gp_RPSSVEP_sil_mex.mki
echo EMC_CONFIG=optim>> gp_RPSSVEP_sil_mex.mki
"C:\Program Files\MATLAB\R2015a\bin\win64\gmake" -B -f gp_RPSSVEP_sil_mex.mk
