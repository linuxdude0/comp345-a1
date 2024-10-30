#include "GameEngineDriver.h"

void testGameEngine(const std::string& map_fileName)
{
    std::cout << "==================TEST GAME ENGINE==================" << std::endl;
    std::string command_tests = "load_map\nvalidate_map\nadd_player\n3\nPlayer1\nPlayer2\nPlayer3\nassign_countries\nissue_order\n4\n5\n6\nend_issue_orders\nexec_orders\nend_exec_orders\nwin\nplay\nquit\n";

    std::cout << ">>[DEBUG]: ==== Running tests for Game Engine class ====" << std::endl;
    GameEngine* gameEngineTest = new GameEngine(map_fileName, 0, 0);
    
    std::istringstream test_input(command_tests);
    // redirect input to std::cin
    std::cin.rdbuf(test_input.rdbuf());


    gameEngineTest->run();
    std::cout << gameEngineTest << std::endl;
    std::cout << "==================END TEST GAME ENGINE==================" << std::endl;
}

void testStartupPhase(void) {
    int argc = 2;
    const char* argv[3] = {
        "",
        "-console",
    };
    GameEngine* ge = new GameEngine("./maps/Atlantis.map", argc, argv);
    ge->run();
    delete ge;
}

void testMainGameLoop(void) {
    int argc = 2;
    const char* argv[3] = {
        "",
        "-file",
        "test_commands.txt",
    };
    GameEngine* ge = new GameEngine("./maps/Atlantis.map", argc, argv);
    ge->run();
    delete ge;
}

