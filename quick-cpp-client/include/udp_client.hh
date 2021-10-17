#include <WinSock2.h>
#include <nlohmann/json.hpp>

namespace quick_client {
    namespace udp {
        using json = nlohmann::json;

        class udp_instance {
            string url;

            void connect(string url);
            void send(event string, json data); 
            void onRecieve(); // function
        }
    }
}