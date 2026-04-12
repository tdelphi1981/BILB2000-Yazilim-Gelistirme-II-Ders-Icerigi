# OBYS v5 Release - Dağıtılabilir Paket

Bu dizin, **Bölüm 8 - Ünite 5 (Kurulum Hazırlama ve Dağıtılabilir Paket)**
için örnek CMake projesidir. Ünite 4'teki `02_obys_v4_final` üzerine
aşağıdaki dağıtım hazırlık bileşenleri eklenmiştir:

- `CMakePresets.json` ile **Debug / Release** profilleri
- **Link Time Optimization (LTO)** Release modunda etkin
- `src/version.h.in` şablonundan **otomatik üretilen `version.h`**
- Qt **LinguistTools** ile çeviri iskeleti (`translations/obys_tr.ts`)
- macOS **bundle** ve Windows **WIN32_EXECUTABLE** ayarları
- `docs/user-guide.md` son kullanıcı kılavuzu

## Derleme

```bash
# Release (LTO açık)
cmake --preset release
cmake --build --preset release

# Debug
cmake --preset debug
cmake --build --preset debug
```

Çıktı: `build/release/obys_v4` (Linux/macOS) veya
`build/release/obys_v4.exe` (Windows).

## Dağıtım Paketi Oluşturma

### macOS

```bash
chmod +x ../deploy_macos.sh
../deploy_macos.sh
```

Bu betik:
1. Release preset ile yapılandırma yapar.
2. Ninja ile derler.
3. `macdeployqt` ile Qt framework'lerini `.app` bundle'ına gömer.
4. DMG üretir ve `hdiutil verify` ile doğrular.
5. Paket boyutunu raporlar.

Çıktı: `build/release/OBYS-v1.0.0-macOS.dmg`

### Windows

```powershell
powershell -ExecutionPolicy Bypass -File ..\deploy_windows.ps1
```

Bu betik:
1. Release preset ile yapılandırma yapar.
2. `cmake --build` ile derler.
3. `windeployqt` ile Qt DLL'lerini ve eklentilerini exe dizinine kopyalar.
4. `ISCC.exe` ile `../obys_installer.iss` betiğini derler.

Çıktı: `Output/OBYS-v1.0.0-Setup.exe`

## Sürüm Bilgisi

`src/version.h.in` CMake `configure_file` ile build dizininde `version.h`
olarak üretilir. `main.cpp` bu dosyayı `#include` ederek
`QCoreApplication::setApplicationVersion(OBYS_VERSION_STRING)` çağrısında
kullanır. "Hakkında" diyaloğu da aynı değeri gösterir.

CMake'deki `project(... VERSION 1.0.0 ...)` satırını güncellediğinde
sürüm otomatik olarak tüm bileşenlere yayılır.
