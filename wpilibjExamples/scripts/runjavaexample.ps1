#---------------------------------------------------------------------------
# Copyright (c) 2018 FIRST. All Rights Reserved.
# Open Source Software - may be modified and shared by FRC teams. The code
# must be accompanied by the FIRST BSD license file in the root directory of
# the project.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#  Invoke a given sample, giving it the PATH and HALSIM_EXTENSIONS we crave.
#----------------------------------------------------------------------------

param ([Parameter(Mandatory=$true)][string]$simname)

function usage() {
    echo "runjavaexample.ps1:  Invoke an FRC CPP Example"
    echo "Usage:"
    echo "  runjavaexample name-of-java-example"
    echo "Specify 'delete' as a name to force a rebuild of your example jar."
}

$t = $env:TEMP
$j = $env:JAVA_HOME


if ($simname -eq "delete") {
    echo "Removing $t/sim*.jar"
    del $t/sim*.jar
    exit
}
$jarname = "$t/sim$simname.jar"

$classdir = (Get-Item -Path $PSScriptRoot\..\classes).FullName
$halbuilddir = (Get-Item -Path $PSScriptRoot\..\..\..\hal\build).FullName
$itestdir = (Get-Item -Path $PSScriptRoot\..\..\..\build\integrationTestFiles).FullName
$wpilibcbuilddir = (Get-Item -Path $PSScriptRoot\..\..\..\wpilibc\build).FullName
$wpilibjdir = (Get-Item -Path $PSScriptRoot\..\..\..\wpilibj\build).FullName
$socketsimdir = (Get-Item -Path $PSScriptRoot\..\..\..\simulation\halsim_ds_socket\build).FullName
$lowfisimdir = (Get-Item -Path $PSScriptRoot\..\..\..\simulation\halsim_lowfi\build).FullName
$simdir = $simname

if (! (Test-Path $simdir)) {
    $simdir = "edu\wpi\first\wpilibj\examples\$simname"
}

if (! (Test-Path $classdir\java\main\$simdir)) {
    echo "Could not find $simname"
    usage
    exit
}

$orig_path = $env:PATH
$p ="$wpilibjdir/libs/wpilibJNIShared/shared/x86-64"
$p ="$halbuilddir/libs/halSim/shared;$p"
$p = "$wpilibcbuilddir/libs/wpilibc/shared/x86-64;$p"
$p = "$wpilibcbuilddir/install/wpilibcTest/x86-64/lib;$p"
$env:PATH = "$p;$orig_path"

$orig_extensions = $env:HALSIM_EXTENSIONS
$e = "$socketsimdir/libs/halsim_ds_socket/shared/halsim_ds_socket.dll"
$e = "$lowfisimdir/libs/halsim_lowfi/shared/halsim_lowfi.dll;$e"
$env:HALSIM_EXTENSIONS = $e

if (! (Test-Path $jarname)) {
    echo "Creating jar file $jarname"
    $m = "Robot-Class: edu.wpi.first.wpilibj.examples.$simname.Robot"
    Out-File -FilePath $t/manifest -InputObject $m -Encoding ASCII

    Push-Location -Path $classdir\java\main
    & $j/bin/jar.exe cfme $jarname $t/manifest edu.wpi.first.wpilibj.RobotBase $simdir
    Pop-Location

    if ((Test-Path $t/simtempdir)) {
        del -recurse -force $t/simtempdir
    }
    mkdir $t/simtempdir >$t/killme
    Push-Location -Path $t/simtempdir
    & $j/bin/jar.exe -xf $itestdir/java/wpilibjIntegrationTests-all.jar
    del -recurse -force META-INF/*
    & $j/bin/jar.exe uf $jarname .
    Pop-Location

}

echo "Running $simname"
& $j/bin/java "-Djava.library.path=$p" -jar $jarname

$env:PATH = $orig_path
$env:HALSIM_EXTENSIONS = $orig_extensions
