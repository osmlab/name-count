#include <iostream>
#include <getopt.h>

#include <osmium/io/any_input.hpp>

#include <osmium/io/any_output.hpp>

#include <sstream>
#include <vector>
#include <set>
#include <boost/format.hpp>

void print_help() {
    std::cout << "name_count INFILE\n\n" \
              << "Outputs a name frequency table.";
}

typedef std::pair<std::string, int> namepair;

class MyCountHandler : public osmium::handler::Handler<MyCountHandler> {

    public:

    std::map<std::string, std::set<long>> names;

    MyCountHandler(const std::string& fieldname) {
    }

    void node(const osmium::Node& node) {
        const char* name = node.tags().get_value_by_key("name");
        if (name) {
            std::string gridsquare = boost::str(boost::format("%1%,%2%") % std::round(node.lon() / 20) % std::round(node.lat() / 20));
            std::string namestr = std::string(node.tags().get_value_by_key("name"));
            for (char& c : namestr) {
                if ((long) c < 0) {
                    std::cout << c << std::endl;
                }
                names[gridsquare].insert((long) c);
            }
        }
    }

    void dump() {
        for (std::map<std::string, std::set<long>>::iterator it = names.begin(); it != names.end(); ++it) {
            std::cout << it->first << ",";
            for (long i : it->second) {
                std::cout << i << ",";
            }
            std::cout << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string input_filename;
    int remaining_args = argc - optind;

    if (remaining_args == 1) {
        input_filename =  argv[optind];
    } else {
        input_filename = "-";
    }

    osmium::io::Reader reader(input_filename);
    osmium::io::Header header = reader.open();
    MyCountHandler count_handler("name");
    reader.apply(count_handler);
    google::protobuf::ShutdownProtobufLibrary();
    std::ostringstream buf;
    count_handler.dump();
}
