Param(
    [Parameter(Mandatory, HelpMessage = "GIVE ME A REBECA!")]
    [string]$Rebec,
    [string]$Prop = $Rebec.Replace('.rebeca', '.property')
)

# Stop if not a rebeca file
if (-Not $Rebec.EndsWith('.rebeca')) {
    Write-Error "A REBECA FILE, MUPP!"
}

# rmc vars
$name = $Rebec.Replace('.\src\', '').Replace('.rebeca', '')
$version = 2.1
$type = 'TIMED_REBECA'
$jar = '.\rmc.jar'
$out = '.\out\' + $name + '\'

# g++ vars
$out_cpp = $out + '*.cpp'
$out_exe = $out + 'executable'
$exe = $out_exe + '.exe'

# process
$executable = @{
    FilePath = "$exe"
    NoNewWindow = $true
    Wait = $true
}

# Finished building the commands, now run them
Write-Host "Running RMC on rebeca"
java -jar $jar -s $Rebec -p $Prop -o $out -v $version -e $type
Write-Output " "

Write-Host "Compiling cpp"
g++ $out_cpp -w -o $out_exe

Write-Host "Starting model check"
Start-Process @executable