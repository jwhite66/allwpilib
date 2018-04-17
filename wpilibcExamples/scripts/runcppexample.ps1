#---------------------------------------------------------------------------
# Copyright (c) 2018 FIRST. All Rights Reserved.
# Open Source Software - may be modified and shared by FRC teams. The code
# must be accompanied by the FIRST BSD license file in the root directory of
# the project.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#  Invoke a given sample, giving it the PATH and HALSIM_EXTENSIONS we crave.
#----------------------------------------------------------------------------

# TODO - Remember JAVA_HOME  and TEMP for JAVA

param ([Parameter(Mandatory=$true)][string]$exename)

function usage() {
    echo "runcppexample.ps1:  Invoke an FRC CPP Example"
    echo "Usage:"
    echo "  runcppexample name-of-cpp-example"
}

$exedir = (Get-Item -Path $PSScriptRoot\..\exe).FullName
$halbuilddir = (Get-Item -Path $PSScriptRoot\..\..\..\hal\build).FullName
$wpilibcbuilddir = (Get-Item -Path $PSScriptRoot\..\..\..\wpilibc\build).FullName
$socketsimdir = (Get-Item -Path $PSScriptRoot\..\..\..\simulation\halsim_ds_socket\build).FullName
$lowfisimdir = (Get-Item -Path $PSScriptRoot\..\..\..\simulation\halsim_lowfi\build).FullName

if (! (Test-Path $exename)) {
    $exename = "$exedir\$exename\x86-64\$exename"
}
if (! (Test-Path $exename)) {
    $exename = "$exename.exe"
}
if (! (Test-Path $exename)) {
    echo "Could not find $exename"
    usage
    exit
}

$orig_path = $env:PATH
$p ="$halbuilddir/libs/halSim/shared;$orig_path"
$p = "$wpilibcbuilddir/libs/wpilibc/shared/x86-64;$p"
$p = "$wpilibcbuilddir/install/wpilibcTestingBaseTest/lib;$p"
$env:PATH = $p

$orig_extensions = $env:HALSIM_EXTENSIONS
$e = "$socketsimdir/libs/halsim_ds_socket/shared/halsim_ds_socket.dll;$orig_extensions"
$e = "$lowfisimdir/libs/halsim_lowfi/shared/halsim_lowfi.dll;$e"
$env:HALSIM_EXTENSIONS = $e


echo $e

echo "Running $exename"
& "$exename"
#&  "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe" /debugexe "$exename"
$env:PATH = $orig_path
$env:HALSIM_EXTENSIONS = $orig_extensions
