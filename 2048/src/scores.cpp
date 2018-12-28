#include "scores.hpp"

bool compare( const Score &a, const Score &b)
{
    return a.score < b.score;
}

void Scoreboard::prompt()
{
    std::cout<< bold_on
             << " Please enter your name to save this score : "<< bold_off;
    std::cin >> name;
}

void Scoreboard::writeToFile()
{
    std::fstream scores("../data/scores.text", std::ios_base::app);

    scores << std::endl
            <<name << " "<< win << " " << moveCount << " "
            << largestTile <<" " << duration;
    endl();
    scores.close();
}

void Scoreboard::printScore()
{
    using std::cout;
    using std::setw;
    readFile();
    clearScreen();
    drawAscii();
    std::cout << green << bold_on << " SCOREBOARD" <<bold_off <<def;
    endl();
    std::cout << green << bold_on << " ——————————" << bold_off << def;
    endl();

    int size = scoreList.size();

    for( int i= size -1 ; i>=0; --i){

        std::string playerName = scoreList[i].name;
        ull playerScore = scoreList[i].score;
        std::string won = scoreList[i].win ? "Yes" : "No";
        long long moveCount = scoreList[i].moveCount;
        ull largestTile = scoreList[i].largestTile;
        double duration = scoreList[i].duration;

        if( i== size -1){
            std::cout << " "
                    <<std::endl;
            endl();
            std::cout << "  | " << bold_on <<"No." <<bold_off <<  " | " <<bold_on
                        << "Name" << "             | " <<bold_on
                        << "Score"<<bold_off <<"   | " <<bold_on <<"Won?"
                        <<bold_off << " | " <<bold_on << "Moves" << bold_off <<" | "
                        <<bold_on <<"Largest Tile" << bold_off << " | " <<bold_on 
                        <<"Duration   " <<bold_off << "  |";
            endl();
            endl();
        }
        cout <<" | " <<setw(2) << size - i << ". | "<<playerName;
        padding(playerName);
        cout << " | " << setw(8) << playerScore << " | " <<setw(4)
            << won << " | " <<setw(5) << moveCount << " | "
            << setw(12) << largestTile << " | " <<setw(12)
            << secondsFormat(duration) << " | " ;
        endl();
        }
    if( !size){
    cout <<" No saved scores."<<endl;
    }
    else 
    endl();
    endl(3);
}

void Scoreboard::printStats()
{
using std::cout;
Stats stats;
if(stats.collectStatistics()){
    cout << green <<bold_on << " STATISTICS" <<bold_off <<def;
    endl();
    cout << green <<bold_on << " ----------" <<bold_off <<def;
    endl();
    endl();
    endl();
    cout << " | " << bold_on << "Best Score     "<< bold_off << " | "
         << std::setw(11) <<stats.bestScore << " |";
    endl();
    cout << " | " << bold_on <<"Game Count    " << bold_off << " | "
        << std::setw(11) <<stats.gameCount << " |";
    endl();
    cout << " | " <<bold_on <<"Number of Wins  " <<bold_off << " | "
        <<std::setw(11) <<stats.winCount << " |";
    endl();
    cout <<" | " << bold_on << "Total Moves Played" << bold_off<<" | "
        << std::setw(11) << stats.totalMoveCount <<" |";
    endl();
    cout <<" | "<< bold_on <<"total Duration   " << bold_off << " | "
        << std::setw(11) << secondsFormat(stats.totalDuration) <<" |";
    endl(3);
}
else{
    std::cout <<"No saved statistics.";
    endl();
}

endl(3);
cout << " press any key to exit:";
char c ;
std::cin>> c;
exit(EXIT_SUCCESS);
}

void Scoreboard::padding(std::string name)
{
    int len = name.length();
    while( 18 -len++)
        std::cout <<" ";
}

void Scoreboard::readFile()
{
    std::ifstream scores("../data/scores.txt");
    if(scores.fail()){
        return;
    }

    std::string playerName;
    ull playerScore;
    bool win;
    ull largestTile;
    long long moveCount;
    double duration;

    while(scores >> playerName >> playerScore >> win >> moveCount>> largestTile >>duration){
        Score bufferScore;
            bufferScore.name = playerName;
            bufferScore.score = playerScore;
            bufferScore.win = win;
            bufferScore.largestTile = largestTile;
            bufferScore.moveCount = moveCount;
            bufferScore.duration = duration;

            scoreList.push_back(bufferScore);
    }
    std::sort(scoreList.begin(),scoreList.end(),compare);
 
}

void Scoreboard::save()
{
    prompt();
    writeToFile();
    std::cout<<green << bold_on << " Score saved !" <<bold_off<< def;
    endl();
}
    

