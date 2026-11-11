param($ExePath)
$cert = Get-ChildItem Cert:\LocalMachine\My | Where-Object {$_.Subject -like "*DesarrolloLocal*"}
Set-AuthenticodeSignature -FilePath $ExePath -Certificate $cert