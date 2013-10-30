#include <iostream>
#include <getopt.h>

#include <osmium/io/any_input.hpp>

#include <osmium/io/any_output.hpp>

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void print_help() {
    std::cout << "name_count INFILE\n\n" \
              << "Outputs a name frequency table.";
}

class MyCountHandler : public osmium::handler::Handler<MyCountHandler> {

    public:

    std::map<const char*, int> names;

    MyCountHandler(const std::string& fieldname) {
    }

    void node(const osmium::Node& node) {
        const char* name = node.tags().get_value_by_key("name");
        if (name) {
            names[name] = names.count(name) == 1 ? (names[name] + 1) : 1;
        }
    }

    void dump() {
        std::map<const char*, int>::iterator iter;
        for (iter = names.begin(); iter != names.end(); ++iter) {
            std::cout << iter->first << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string input_filename;
    std::string output_filename("ogr_out");
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
