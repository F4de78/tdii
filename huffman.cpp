
#include "huffman.h"

using namespace std;


/********
-- Utility --
*********/

//'efficent' way to count all chars in a file; seek to the end of file,
// save the number of chars and then seeks to the beginning of file
unsigned int wc(ifstream &in){
    unsigned int chars;
    in.seekg(0,in.end);
    streampos end_pos = in.tellg();
    chars = int(end_pos);
    in.seekg(0);

    return chars;
}

//for debug: print the map
void printMap(myMap m){
    myMap::iterator itrP;
    for (itrP = m.begin(); itrP != m.end(); ++itrP) {
            std::cout << itrP->first
              << ':'
              << itrP->second
              << std::endl;
    }
    cout << endl;
}

//for debug: print the queue
void printPQ(myPriorityQueue gq){
    myPriorityQueue g = gq;
    while (!g.empty()) {
        pair<double,char> top = g.top()->info;
        cout << "k:" << top.first << "\t";
        cout << "v:" << top.second << endl;
        g.pop();
    }
    cout << endl;
}

//count the chars occurency of a file, saves in a file and calculate the frequency of each char
myMap getFrequencyInFile (string filename){
    ifstream in (filename);
    char c;
    myMap m;
    unsigned int chars = wc(in);
#ifdef DEBUG
    cout << chars << endl;
#endif
    //fill map with the occurency
    while (in.get(c) && in.is_open()) {
        if(m.count(c) > 0)
            m.at(c)++;
        else
            m.insert(mapPairs(c,1));
    }
    //calculate the frequency of each character
    myMap::iterator itr;
    for (itr = m.begin(); itr != m.end(); ++itr) {
        itr->second = itr->second/chars;
    }
#ifdef DEBUG
    printMap(m);
#endif
    return m;
}

//convert map in a Priority Queue of nodes
myPriorityQueue createPQ (myMap m){
    myPriorityQueue pq;
    
    myMap::iterator itr;
    for (itr = m.begin(); itr != m.end(); ++itr) {
        pq.push(createNewNode(make_pair(itr->second,itr->first)));
    }
#ifdef DEBUG
    printPQ(pq);
#endif
    return pq;
}

double ShannonEntropy (myMap m){
    myMap::iterator itrP;
    double se;
    for (itrP = m.begin(); itrP != m.end(); ++itrP) {
        se += itrP->second * log2(1/itrP->second);
    }
    return se;
}

void HuffmanCode(myPriorityQueue &pq){

    uint pqsize = pq.size(); 
    for(uint i = 0; i < pqsize-1; i++){
        
        Node x = pq.top();
        cout << "x:"<< pq.top()->info.first ;
        pq.pop();
        Node y = pq.top();
        cout << " ~~~~ y:"<< pq.top()->info.first ;
        pq.pop();
        Node z = sumNodesValues(x,y);
        cout << " ~~~~ z:"<< z->info.first << endl;
        leftInsert(z,x);
        rightInsert(z,y);

        pq.push(z);
    }
    
}

int main(int argc,char** argv){
    myMap m;
    m = getFrequencyInFile(argv[1]);
    myPriorityQueue pq = createPQ(m);
    cout << "Shannon entropy of "<< argv[1] << ": " << ShannonEntropy(m) << endl;
    cout << "size of pq: " << pq.size() << endl;
    HuffmanCode(pq);
    printPQ(pq);
    cout << "size of pq: " << pq.size() << endl;
    printPaths(pq.top());
}
