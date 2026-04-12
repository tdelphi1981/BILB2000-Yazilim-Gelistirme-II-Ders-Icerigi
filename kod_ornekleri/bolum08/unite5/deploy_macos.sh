#!/usr/bin/env bash
#
# deploy_macos.sh -- OBYS v5 icin macOS dagitim paketi olusturma betigi
#
# Bolum 8 - Unite 5 - Kurulum Hazirlama ve Dagitilabilir Paket
#
# Bu betik asagidaki adimlari sirayla calistirir:
#   1) Release preset ile CMake yapilandirmasi
#   2) Ninja ile optimizasyonlu derleme
#   3) macdeployqt ile Qt bagimliliklarinin .app paketine gomulmesi
#   4) DMG (Disk Image) dosyasinin uretilmesi
#   5) hdiutil ile DMG butunluk dogrulamasi
#   6) Son paket boyutu raporu
#
# Kullanim:
#   chmod +x deploy_macos.sh
#   ./deploy_macos.sh
#
# On kosul:
#   - Qt 6.5+ kurulu ve PATH'te olmali (ornegin /opt/Qt/6.7.0/macos/bin)
#   - cmake, ninja ve macdeployqt erisilebilir olmali

set -euo pipefail

# --- Ayarlar ---------------------------------------------------------------
PROJE_KOK="02_obys_v5_release"
UYGULAMA_AD="obys_v4"
BUILD_DIR="${PROJE_KOK}/build/release"
APP_BUNDLE="${BUILD_DIR}/${UYGULAMA_AD}.app"
DMG_AD="OBYS-v1.0.0-macOS.dmg"

# --- Renkli log ------------------------------------------------------------
log_info()  { printf "\033[1;34m[INFO]\033[0m %s\n" "$1"; }
log_ok()    { printf "\033[1;32m[OK]\033[0m   %s\n" "$1"; }
log_err()   { printf "\033[1;31m[HATA]\033[0m %s\n" "$1" >&2; }

# --- 1) CMake yapilandirma -------------------------------------------------
log_info "CMake release preset yapilandiriliyor..."
cmake --preset release -S "${PROJE_KOK}"
log_ok "Yapilandirma tamamlandi."

# --- 2) Derleme ------------------------------------------------------------
log_info "Proje derleniyor (release)..."
cmake --build --preset release
log_ok "Derleme tamamlandi."

if [[ ! -d "${APP_BUNDLE}" ]]; then
    log_err "App bundle bulunamadi: ${APP_BUNDLE}"
    exit 1
fi

# --- 3) macdeployqt --------------------------------------------------------
log_info "macdeployqt ile Qt bagimliliklari gomuluyor..."
macdeployqt "${APP_BUNDLE}" -dmg -verbose=1
log_ok "macdeployqt tamamlandi."

# macdeployqt uretilen DMG'yi app'in yaninda birakir
URETILEN_DMG="${BUILD_DIR}/${UYGULAMA_AD}.dmg"
if [[ ! -f "${URETILEN_DMG}" ]]; then
    log_err "DMG bulunamadi: ${URETILEN_DMG}"
    exit 1
fi

# Dagitim adina tasi
mv "${URETILEN_DMG}" "${BUILD_DIR}/${DMG_AD}"
FINAL_DMG="${BUILD_DIR}/${DMG_AD}"

# --- 4) DMG dogrulama ------------------------------------------------------
log_info "DMG butunlugu dogrulaniyor..."
hdiutil verify "${FINAL_DMG}"
log_ok "DMG dogrulama basarili."

# --- 5) Boyut raporu -------------------------------------------------------
APP_BOYUT=$(du -sh "${APP_BUNDLE}" | awk '{print $1}')
DMG_BOYUT=$(du -sh "${FINAL_DMG}" | awk '{print $1}')

echo
log_ok "Dagitim paketi hazir!"
echo "  .app bundle: ${APP_BUNDLE} (${APP_BOYUT})"
echo "  DMG paketi : ${FINAL_DMG} (${DMG_BOYUT})"
echo
