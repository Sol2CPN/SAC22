#include <fstream>
#include <iostream>
#include <sstream>

#include "include/nlohmann/json.hpp"
#include "include/ASTAnalyser.hpp"
#include "include/Constants.hpp"
#include "include/Utils.hpp"

#include "include/Helena.hpp"
#include "include/Translator.hpp"

int main(int argc, char** argv){
    
    std::string ast_file_name = "./test_files/blindAuction.ast";
    std::string ast_json_file_name = "./test_files/blindAuction.json";
    std::string output_file_name = "./test_files/blindAuction_generated.lna";

    std::ifstream ast_text_file_stream(ast_file_name);
    std::ifstream ast_json_file_stream(ast_json_file_name);

    std::stringstream ast_text_stream;



    std::string new_line;
    std::string sol_name;
    std::string ast_json_content;

    while (getline(ast_text_file_stream, new_line)) {
        ast_text_stream << new_line << "\n";
    }

    while (getline(ast_json_file_stream, new_line)) {
        if (new_line.find(".sol =======") != std::string::npos) {
            sol_name = SOL2CPN::Utils::substr_by_edge(new_line, "======= ", " =======");
            break;
        }
    }

    while (getline(ast_json_file_stream, new_line)) {
        if (new_line.find(".sol =======") == std::string::npos) {
            ast_json_content = ast_json_content + new_line + "\n";
        } else {
            nlohmann::json ast_json = nlohmann::json::parse(ast_json_content);
            //SOL2CPN::ASTAnalyser ast_analyser(ast_json, sol_name);
            //std::stringstream new_source = ast_analyser.analyse();
            //std::cout << sol_name << " " << ast_json.at("absolutePath") << std::endl;
            sol_name = SOL2CPN::Utils::substr_by_edge(new_line, "", "");
            ast_json_content = "";
        }
    }


    if (ast_json_content != "") {
        nlohmann::json ast_json = nlohmann::json::parse(ast_json_content);
        SOL2CPN::ASTAnalyser ast_analyser(ast_text_stream, ast_json, true, sol_name, "");

        SOL2CPN::RootNodePtr root_node = ast_analyser.analyse();

        SOL2CPN::Translator nettranslator(root_node);

        SOL2CPN::NetNodePtr net_node = nettranslator.translate();

       
      //  SOL2CPN::Translator nettranslator(root_node);
       
       // SOL2CPN::NetNodePtr net_node = nettranslator.translate();
        //SOL2CPN::Indentation indentation;
        //std::string new_source = root_node->source_code(indentation);
        //std::cout << sol_name << " " << ast_json.at("absolutePath") << std::endl;
        /*if (output_file_name != "") {
            std::ofstream output_file_stream(output_file_name);
            output_file_stream << new_source;
            output_file_stream.close();
        } else {
        }*/

        std::string new_source = net_node->source_code();
        if (output_file_name != "") {
            std::ofstream output_file_stream(output_file_name);
            output_file_stream << new_source;
            output_file_stream.close();
        } else {
        }



    }
    
    ast_json_file_stream.close();

    exit(SOL2CPN::ErrorCode::SUCCESS);

    return 0;
}