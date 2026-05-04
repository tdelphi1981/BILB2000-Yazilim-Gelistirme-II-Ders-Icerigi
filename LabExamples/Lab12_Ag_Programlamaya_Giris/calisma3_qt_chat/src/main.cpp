/**
 * Lab 12 - Çalışma 3: Qt TCP Chat Sunucusu - main
 *
 * Test:
 *   ./qt_chat_sunucu                  (port 9100'de baslar)
 *
 * Istemci tarafinda length-prefix gerekli oldugu icin nc dogrudan
 * calismaz. Hizli test icin Python yardimcisi:
 *
 *   python3 -c "
 *   import socket, struct, sys, threading
 *   s = socket.socket(); s.connect(('127.0.0.1', 9100))
 *   def oku():
 *       while True:
 *           h = s.recv(4)
 *           if not h: break
 *           n = struct.unpack('>I', h)[0]
 *           print('<<<', s.recv(n).decode())
 *   threading.Thread(target=oku, daemon=True).start()
 *   for satir in sys.stdin:
 *       p = satir.rstrip().encode()
 *       s.sendall(struct.pack('>I', len(p)) + p)
 *   "
 */

#include "ChatSunucu.h"

#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    quint16 port = (argc > 1) ? static_cast<quint16>(std::atoi(argv[1])) : 9100;

    ChatSunucu sunucu;
    if (!sunucu.baslat(port)) {
        return 1;
    }

    return app.exec();
}
