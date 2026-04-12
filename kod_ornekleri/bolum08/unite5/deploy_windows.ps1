# deploy_windows.ps1 -- OBYS v5 icin Windows dagitim paketi olusturma betigi
#
# Bolum 8 - Unite 5 - Kurulum Hazirlama ve Dagitilabilir Paket
#
# Bu PowerShell betigi asagidaki adimlari sirayla calistirir:
#   1) Release preset ile CMake yapilandirmasi
#   2) cmake --build ile optimizasyonlu derleme
#   3) windeployqt ile Qt bagimliliklarinin exe dizinine kopyalanmasi
#   4) Inno Setup (iscc.exe) ile kurulum dosyasi uretimi
#
# Kullanim:
#   powershell -ExecutionPolicy Bypass -File deploy_windows.ps1
#
# On kosul:
#   - Qt 6.5+ kurulu ve PATH'te (windeployqt.exe erisilebilir)
#   - cmake ve ninja (veya Visual Studio generator) PATH'te
#   - Inno Setup 6 kurulu: C:\Program Files (x86)\Inno Setup 6\ISCC.exe

$ErrorActionPreference = "Stop"

# --- Ayarlar ---------------------------------------------------------------
$ProjeKok    = "02_obys_v5_release"
$UygulamaAd  = "obys_v4"
$BuildDir    = Join-Path $ProjeKok "build\release"
$ExePath     = Join-Path $BuildDir "$UygulamaAd.exe"
$InnoScript  = "obys_installer.iss"
$IsccPath    = "C:\Program Files (x86)\Inno Setup 6\ISCC.exe"

function Log-Info($msg) { Write-Host "[INFO] $msg" -ForegroundColor Cyan }
function Log-Ok($msg)   { Write-Host "[OK]   $msg" -ForegroundColor Green }
function Log-Err($msg)  { Write-Host "[HATA] $msg" -ForegroundColor Red }

# --- 1) CMake yapilandirma -------------------------------------------------
Log-Info "CMake release preset yapilandiriliyor..."
cmake --preset release -S $ProjeKok
if ($LASTEXITCODE -ne 0) {
    Log-Err "CMake yapilandirma basarisiz."
    exit 1
}
Log-Ok "Yapilandirma tamamlandi."

# --- 2) Derleme ------------------------------------------------------------
Log-Info "Proje derleniyor (release)..."
cmake --build --preset release
if ($LASTEXITCODE -ne 0) {
    Log-Err "Derleme basarisiz."
    exit 1
}
Log-Ok "Derleme tamamlandi."

if (-not (Test-Path $ExePath)) {
    Log-Err "Exe dosyasi bulunamadi: $ExePath"
    exit 1
}

# --- 3) windeployqt --------------------------------------------------------
Log-Info "windeployqt ile Qt bagimliliklari kopyalaniyor..."
windeployqt --release --no-translations --no-system-d3d-compiler `
    --no-opengl-sw --compiler-runtime $ExePath
if ($LASTEXITCODE -ne 0) {
    Log-Err "windeployqt basarisiz."
    exit 1
}
Log-Ok "windeployqt tamamlandi."

# --- 4) Inno Setup kurulum ureticisi ---------------------------------------
if (-not (Test-Path $IsccPath)) {
    Log-Err "Inno Setup bulunamadi: $IsccPath"
    Log-Err "Lutfen https://jrsoftware.org/isinfo.php adresinden kurun."
    exit 1
}

Log-Info "Inno Setup ile kurulum dosyasi uretiliyor..."
& $IsccPath $InnoScript
if ($LASTEXITCODE -ne 0) {
    Log-Err "Inno Setup derleme basarisiz."
    exit 1
}
Log-Ok "Kurulum dosyasi hazir: Output\OBYS-v1.0.0-Setup.exe"

Write-Host ""
Log-Ok "Dagitim paketi olusturuldu."
