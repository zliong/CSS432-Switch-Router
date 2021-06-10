#include <iostream>
#include <cstdint>
#include <iterator>
#include <map>
#include <random>
using namespace std;

struct ipv4{
    unsigned int ver: 4;
    unsigned int traffic: 8;
    unsigned int flow_label: 20;
    unsigned int payload_length: 16;
    unsigned int next_header: 8;
    unsigned int hop_limit: 8;
    // struct address source;
    // struct address dest;
    uint32_t sfield[4];
    uint32_t dfield[4];
    void * payload;
};
struct ipv6{
    unsigned int ver: 4;
    unsigned int traffic: 8;
    unsigned int flow_label: 20;
    unsigned int payload_length: 16;
    unsigned int next_header: 8;
    unsigned int hop_limit: 8;
    // struct address source;
    // struct address dest;
    uint16_t sfield[8];
    uint16_t dfield[8];
    void * payload;
};
//Used specifically for ipv4 (since it really only uses 8 bits per field)
struct tblEntry{
    uint32_t addr[4];
    int submask;
    int port;
    string strAddr;
};
struct Entry{
    uint16_t addr[8];
    int submask;
    int port;
    string strAddr;
};
//A struct of the routing table for ipv6
struct ip_table{
    tblEntry entries[5];
};
struct ipv6_table
{
    Entry entries[5];
};
//A hard-coded intilization of ip routing table (ipv4)
void initializeIPv4RoutingTable(ip_table *tbl_ptr)
{
    tbl_ptr->entries[0].addr[0] = 192;
    tbl_ptr->entries[0].addr[1] = 168;
    tbl_ptr->entries[0].addr[2] = 0;
    tbl_ptr->entries[0].addr[3] = 0;
    tbl_ptr->entries[0].port = 1;
    tbl_ptr->entries[0].submask = 14;
    tbl_ptr->entries[0].strAddr = "192.168.0.0/14";

    tbl_ptr->entries[1].addr[0] = 192;
    tbl_ptr->entries[1].addr[1] = 168;
    tbl_ptr->entries[1].addr[2] = 1;
    tbl_ptr->entries[1].addr[3] = 0;
    tbl_ptr->entries[1].port = 2;
    tbl_ptr->entries[1].submask = 24;
    tbl_ptr->entries[1].strAddr = "192.168.1.0/24";

    tbl_ptr->entries[2].addr[0] = 192;
    tbl_ptr->entries[2].addr[1] = 168;
    tbl_ptr->entries[2].addr[2] = 2;
    tbl_ptr->entries[2].addr[3] = 0;
    tbl_ptr->entries[2].port = 3;
    tbl_ptr->entries[2].submask = 24;
    tbl_ptr->entries[2].strAddr = "192.168.2.0/24";

    tbl_ptr->entries[3].addr[0] = 192;
    tbl_ptr->entries[3].addr[1] = 168;
    tbl_ptr->entries[3].addr[2] = 2;
    tbl_ptr->entries[3].addr[3] = 0;
    tbl_ptr->entries[3].port = 4;
    tbl_ptr->entries[3].submask = 26;
    tbl_ptr->entries[3].strAddr = "192.168.2.0/26";

    tbl_ptr->entries[4].addr[0] = 0;
    tbl_ptr->entries[4].addr[1] = 0;
    tbl_ptr->entries[4].addr[2] = 0;
    tbl_ptr->entries[4].addr[3] = 0;
    tbl_ptr->entries[4].port = 5;
    tbl_ptr->entries[4].submask = 0;
    tbl_ptr->entries[4].strAddr = "0.0.0.0/0";
}

// IPv6 Intialization, Global Ipv6 Routing Table
void initializeIPv6RoutingTable(ipv6_table *tbl_ptr)
{
    tbl_ptr->entries[0].addr[0] = 8193;
    tbl_ptr->entries[0].addr[1] = 3512;
    tbl_ptr->entries[0].addr[2] = 4660;
    tbl_ptr->entries[0].addr[3] = 0;
    tbl_ptr->entries[0].addr[4] = 0;
    tbl_ptr->entries[0].addr[5] = 0;
    tbl_ptr->entries[0].addr[6] = 0;
    tbl_ptr->entries[0].addr[7] = 0;
    tbl_ptr->entries[0].port = 1;
    tbl_ptr->entries[0].submask = 48;
    tbl_ptr->entries[0].strAddr = "2001:0BD8:1234::/48";

    tbl_ptr->entries[1].addr[0] = 0;
    tbl_ptr->entries[1].addr[1] = 0;
    tbl_ptr->entries[1].addr[2] = 0;
    tbl_ptr->entries[1].addr[3] = 0;
    tbl_ptr->entries[1].addr[4] = 0;
    tbl_ptr->entries[1].addr[5] = 0;
    tbl_ptr->entries[1].addr[6] = 0;
    tbl_ptr->entries[1].addr[7] = 1;
    tbl_ptr->entries[1].port = 2;
    tbl_ptr->entries[1].submask = 128;
    tbl_ptr->entries[1].strAddr = "::1/128";

    tbl_ptr->entries[2].addr[0] = 8193;
    tbl_ptr->entries[2].addr[1] = 3512;
    tbl_ptr->entries[2].addr[2] = 4660;
    tbl_ptr->entries[2].addr[3] = 1;
    tbl_ptr->entries[2].addr[4] = 1;
    tbl_ptr->entries[2].addr[5] = 0;
    tbl_ptr->entries[2].addr[6] = 0;
    tbl_ptr->entries[2].addr[7] = 0;
    tbl_ptr->entries[2].port = 3;
    tbl_ptr->entries[2].submask = 64;
    tbl_ptr->entries[2].strAddr = "2001:0BD8:1234:1:1::/64";

    tbl_ptr->entries[3].addr[0] = 8193;
    tbl_ptr->entries[3].addr[1] = 3512;
    tbl_ptr->entries[3].addr[2] = 4660;
    tbl_ptr->entries[3].addr[3] = 1;
    tbl_ptr->entries[3].addr[4] = 1;
    tbl_ptr->entries[3].addr[5] = 1;
    tbl_ptr->entries[3].addr[6] = 1;
    tbl_ptr->entries[3].addr[7] = 1;
    tbl_ptr->entries[3].port = 4;
    tbl_ptr->entries[3].submask = 128;
    tbl_ptr->entries[3].strAddr = "2001:0BD8:1234::1/128";
    
    tbl_ptr->entries[4].addr[0] = 0;
    tbl_ptr->entries[4].addr[1] = 0;
    tbl_ptr->entries[4].addr[2] = 0;
    tbl_ptr->entries[4].addr[3] = 0;
    tbl_ptr->entries[4].addr[4] = 0;
    tbl_ptr->entries[4].addr[5] = 0;
    tbl_ptr->entries[4].addr[6] = 0;
    tbl_ptr->entries[4].addr[7] = 0;
    tbl_ptr->entries[4].port = 0;
    tbl_ptr->entries[4].submask = 0;
    tbl_ptr->entries[4].strAddr = "::/0";
}
//visually display on console (ipv4 style)
void displayIPv4RoutingTable(ip_table *ptr)
{
    cout << "Network Address(IPv4)         Mask         Interface" << endl;
    cout << "----------------------------------------------------" << endl;
    for(int i = 0; i < 5;i++)
    {
        if(i == 4)
        {
            cout << ptr->entries[i].strAddr << "                        " << ptr->entries[i].submask << "                 " << ptr->entries[i].port << endl;
            break;
        }
        else
        {
            cout << ptr->entries[i].strAddr << "                  " << ptr->entries[i].submask << "                 " << ptr->entries[i].port << endl;
        }
    }
    cout << "----------------------------------------------------" << endl;
    cout << endl << endl;
}
//find if the ipv4 destination matches the routing table's (bit by bit)
bool findIPv4Match(ipv4 packet,uint32_t curEntry[], int submask)
{
    bool trueMatch = true;
    int index = 0;
    if(submask < 8)
    {
        index = 0;
    }
    else if((submask % 8) == 0) //means that it is within its field
    {
        index = (submask / 8) - 1;
    }
    else{   //otherwise, its actually has extra bits to check
        index = (submask / 8) ;
    }
    // cout << "In Match" << endl;
    // cout << submask << endl;
    // cout << "Index " << index << endl;
    while(submask > 0)
    {
        //cout << "In Match Loop" << endl;
        uint32_t mask = 128;    //in binary: 10000000
        for(int k = 0; k <= index; k++)
        {
            //cout << "Enter the first for loop!" << endl;
            for(int j = 0; j <= 7; j++)
            {
                if((curEntry[k] & mask) == (packet.dfield[k] & mask))
                {
                    submask--;
                    mask = mask >> 1;   //i.e. 01000000, 00100000
                } 
                else 
                {
                    return false;
                }
            }
            mask = 128;
        }
    }
    if(trueMatch == false)
    {
        return trueMatch;
    } 
    else 
    {
        return trueMatch;
    }
}
//Searches for the closest interface or matching bits (ipv4)
int longestIPv4Match(ipv4 packet, ip_table* tbl_ptr)
{
    //default port if no match is found
    int port = 5;
    int longestMatch = 0;
    bool match;
    //cout << "In Longest" << endl;
    for(int i = 0; i < 5; i++)
    {
        //cout << "In Longest Loop" << endl;
        match = findIPv4Match(packet, tbl_ptr->entries[i].addr, tbl_ptr->entries[i].submask);
        if(match == true && tbl_ptr->entries[i].submask >= longestMatch)
        {
            port = tbl_ptr->entries[i].port;
            longestMatch = tbl_ptr->entries[i].submask;
            //cout<< "Current Longest Match (Submask): "<< longestMatch << endl;
        }
        match = false;
    }
    //cout<< "Current Longest Match (Submask): "<< longestMatch << endl;
    return port;

}

//Utilizes the entire all 128 bits within the field, check 16-bits for 8 fields total. Checks for matching bits
bool findIPv6Match(ipv6 packet,uint16_t curEntry[], int submask)
{
    bool trueMatch = true;
    int index = 0;
    if(submask < 16)    //means that it is within its first field
    {
        index = 0;
    }
    else if((submask % 16) == 0)    //Means it stays within that field from 0 to that num
    {
        index = (submask / 16) - 1;
    }
    else
    {
        index = (submask / 16); //Means that there are a few extra bits to check out beyond its max field limit
    }
    while(submask > 0)
    {
        //cout << "In Match Loop" << endl;
        uint32_t mask = 32768;    //in binary: 1000000000000000
        for(int k = 0; k <= index; k++)
        {
            //cout << "Enter the first for loop!" << endl;
            for(int j = 0; j <= 15; j++)
            {
                //Check each bit, if match, then increase count
                if((curEntry[k] & mask) == (packet.dfield[k] & mask))
                {
                    //cout << "In checking Loop" << endl;
                    submask--;
                    mask = mask >> 1;   //i.e. 01000000, 00100000
                } 
                else 
                {
                    return false;
                }
            }
            mask = 32768;   //reset the value
        }
    }
    if(trueMatch == false)
    {
        return trueMatch;
    } 
    else 
    {
        return trueMatch;
    }
}
//Searches for the nearest match to port/mask
int longestIPv6Match(ipv6 packet, ipv6_table* tbl_ptr)
{
    //default port if no match is found
    int port = 5;
    int longestMatch = 0;
    bool match;
    //cout << "In Longest" << endl;
    for(int i = 0; i < 5; i++)
    {
        //cout << "In Longest Loop" << endl;
        match = findIPv6Match(packet, tbl_ptr->entries[i].addr, tbl_ptr->entries[i].submask);
        if(match == true && tbl_ptr->entries[i].submask >= longestMatch)    //Means that it is greater than its current submask and that they found that they matched
        {
            port = tbl_ptr->entries[i].port;
            longestMatch = tbl_ptr->entries[i].submask;
            //cout<< "Current Longest Match (Submask): "<< longestMatch << endl;
        }
        match = false;
    }
    //cout<< "Current Longest Match (Submask): "<< longestMatch << endl;
    return port;

}
void displayIPv6RoutingTable(ipv6_table *ptr)
{
    cout << "Network Address(IPv6)          Mask             Interface" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << ptr->entries[0].strAddr << "             " << ptr->entries[0].submask << "                      " << ptr->entries[0].port << endl;
    cout << ptr->entries[1].strAddr << "                        " << ptr->entries[1].submask << "                      " << ptr->entries[1].port << endl;
    cout << ptr->entries[3].strAddr << "          " << ptr->entries[3].submask << "                      " << ptr->entries[3].port << endl;
    cout << ptr->entries[4].strAddr << "                             " << ptr->entries[4].submask << "                      " << ptr->entries[4].port << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << endl << endl;
}
int main()
{
    //create the pre-defined fowarding table
    //initializeRoutingTable();
    ip_table table;
    initializeIPv4RoutingTable(&table);
    displayIPv4RoutingTable(&table);
    ipv4 packets[7];
    cout << "Generating arbitrary packets......................." << endl;
    cout << "Packets            Src Address         Dest Address" << endl;
    cout << "---------------------------------------------------" << endl;
    random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> randomInt(0,2);    // set up the range
    std::uniform_int_distribution<> randomGreaterInt(10,255);  //set up the range
    std::uniform_int_distribution<> randomHopInt(0, 15);  //set up the hop range
    //generate 5 random IPv4 Packet
    for(int i = 0; i < 5; i++)
    {
        int j = 0;
        packets[i].ver = 1;
        packets[i].traffic = randomGreaterInt(gen);
        packets[i].next_header = randomGreaterInt(gen);
        packets[i].flow_label = randomInt(gen);
        packets[i].payload_length = randomGreaterInt(gen);
        packets[i].dfield[0] = 192;
        packets[i].dfield[1] = 168;
        packets[i].dfield[2] = randomInt(gen);
        packets[i].dfield[3] = randomGreaterInt(gen);
        packets[i].hop_limit = randomHopInt(gen);   //give an arbitrary value from (0-F) or (0-15)
        packets[i].sfield[0] = randomGreaterInt(gen);
        packets[i].sfield[1] = randomGreaterInt(gen);
        packets[i].sfield[2] = randomGreaterInt(gen);
        packets[i].sfield[3] = randomGreaterInt(gen);
        cout << i <<"              " << packets[i].sfield[j] << "." <<packets[i].sfield[j+1] <<"." << packets[i].sfield[j+2]<<"."<< packets[i].sfield[j+3] <<"        " << 
        packets[i].dfield[j] << "." << packets[i].dfield[j+1] <<"." << packets[i].dfield[j+2]<<"."<< packets[i].dfield[j+3] << endl;
    }
        //created a packet to test out for interface 4
        packets[5].dfield[0] = 192;
        packets[5].dfield[1] = 168;
        packets[5].dfield[2] = 2;
        packets[5].dfield[3] = 0;
        packets[5].ver = 1;
        packets[5].traffic = randomGreaterInt(gen);
        packets[5].next_header = randomGreaterInt(gen);
        packets[5].flow_label = randomInt(gen);
        packets[5].payload_length = randomGreaterInt(gen);
        packets[5].sfield[0] = randomGreaterInt(gen);
        packets[5].sfield[1] = randomGreaterInt(gen);
        packets[5].sfield[2] = randomGreaterInt(gen);
        packets[5].sfield[3] = randomGreaterInt(gen);
        cout << 5 <<"              " << packets[5].sfield[0] << "." <<packets[5].sfield[1] <<"." << packets[5].sfield[2]<<"."<< packets[5].sfield[3] <<"        " << 
        packets[5].dfield[0] << "." << packets[5].dfield[1] <<"." << packets[5].dfield[2]<<"."<< packets[5].dfield[3] << endl;
        //created a packet to test out for interface 0
        packets[6].dfield[0] = 124;
        packets[6].dfield[1] = 255;
        packets[6].dfield[2] = 2;
        packets[6].dfield[3] = 186;
        packets[6].ver = 1;
        packets[6].traffic = randomGreaterInt(gen);
        packets[6].next_header = randomGreaterInt(gen);
        packets[6].flow_label = randomInt(gen);
        packets[6].payload_length = randomGreaterInt(gen);
        packets[6].sfield[0] = randomGreaterInt(gen);
        packets[6].sfield[1] = randomGreaterInt(gen);
        packets[6].sfield[2] = randomGreaterInt(gen);
        packets[6].sfield[3] = randomGreaterInt(gen);
        cout << 6 <<"              " << packets[6].sfield[0] << "." <<packets[6].sfield[1] <<"." << packets[6].sfield[2]<<"."<< packets[6].sfield[3] <<"        " << 
        packets[6].dfield[0] << "." << packets[6].dfield[1] <<"." << packets[6].dfield[2]<<"."<< packets[6].dfield[3] << endl;
    cout << endl << endl;
    //Tests out ipv4 packets
    cout << "Testing where IPv4 Packets are going............................" << endl;
    for(int i = 0; i < 7; i++)
    {
        int j = 0;
        if(i < 5)
        {
            packets[i].hop_limit -= 1;  //Update Hop Limit by decrementing it by 1; if hop goes below 0, drop it
        }
        if(packets[i].hop_limit < 0)
        {
            cout << "Packet[" << i << "] was dropped due to expired hop limit!!" << endl;
        }
        else
        {
            cout <<"Packet[" << i <<"] is going from " << packets[i].sfield[j] << "." <<packets[i].sfield[j+1] <<"." << packets[i].sfield[j+2]<<"."<< packets[i].sfield[j+3] <<" to " << packets[i].dfield[j]
            << "." << packets[i].dfield[j+1] <<"." << packets[i].dfield[j+2]<<"."<< packets[i].dfield[j+3] <<" routed on interface " << longestIPv4Match(packets[i], &table) << endl;
        }

    }
    cout << endl << endl;
    ipv6_table itable;
    initializeIPv6RoutingTable(&itable);
    displayIPv6RoutingTable(&itable);
    ipv6 packets6[8];
    cout << "Generating arbitrary packets..................................." << endl;
    cout << "Packets            Src Address                     Dest Address" << endl;
    cout << "---------------------------------------------------------------" << endl;
    //Generate 5 ipv6 packets
    for(int i = 0; i < 5; i++)
    {
        int j = 0;
        packets6[i].ver = 1;
        packets6[i].traffic = randomGreaterInt(gen);
        packets6[i].next_header = randomGreaterInt(gen);
        packets6[i].flow_label = randomInt(gen);
        packets6[i].payload_length = randomGreaterInt(gen);
        packets6[i].dfield[0] = 8193;
        packets6[i].dfield[1] = 3512;
        packets6[i].dfield[2] = 4660;
        packets6[i].dfield[3] = randomInt(gen);
        packets6[i].dfield[4] = randomInt(gen);
        packets6[i].dfield[5] = randomInt(gen);
        packets6[i].dfield[6] = 1;
        packets6[i].dfield[7] = 1;
        packets6[i].hop_limit = randomHopInt(gen);  //give an arbitrary value from (0-F) or (0-15)
        packets6[i].sfield[0] = randomGreaterInt(gen);
        packets6[i].sfield[1] = randomGreaterInt(gen);
        packets6[i].sfield[2] = randomGreaterInt(gen);
        packets6[i].sfield[3] = randomGreaterInt(gen);
        packets6[i].sfield[3] = randomInt(gen);
        packets6[i].sfield[4] = randomInt(gen);
        packets6[i].sfield[5] = randomInt(gen);
        packets6[i].sfield[6] = randomInt(gen);
        packets6[i].sfield[7] = randomInt(gen);
        cout << std::hex << i <<"                " << packets6[i].sfield[j] << ":" <<packets6[i].sfield[j+1] <<":" << packets6[i].sfield[j+2]<<":"<< packets6[i].sfield[j+3] <<"::          " << 
        packets6[i].dfield[j] << ":" << packets6[i].dfield[j+1] <<":" << packets6[i].dfield[j+2]<<":"<< packets6[i].dfield[j+3] << ":" << packets6[i].dfield[j+4] << ":" << packets6[i].dfield[j+5] << ":" << packets6[i].dfield[j+6]
         << ":" << packets6[i].dfield[j+7] << endl;
    }
    //created a packet to test out for interface 4
    packets6[5].ver = 1;
    packets6[5].dfield[0] = 1193;
    packets6[5].dfield[1] = 3512;
    packets6[5].dfield[2] = 4660;
    packets6[5].dfield[3] = randomInt(gen);
    packets6[5].dfield[4] = randomInt(gen);
    packets6[5].dfield[5] = randomInt(gen);
    packets6[5].dfield[6] = randomInt(gen);
    packets6[5].dfield[7] = randomInt(gen);
    cout << std::hex << 5 <<"                " << packets6[5].sfield[0] << ":" <<packets6[5].sfield[1] <<":" << packets6[5].sfield[2]<<":"<< packets6[5].sfield[3] <<"::                " << 
    packets6[5].dfield[0] << ":" << packets6[5].dfield[1] <<":" << packets6[5].dfield[2]<<":"<< packets6[5].dfield[3] << ":" << packets6[5].dfield[4] << ":" << packets6[5].dfield[6] 
    << ":" << packets6[5].dfield[7] << endl;

    packets6[6].ver = 1;
    packets6[6].dfield[0] = 8193;
    packets6[6].dfield[1] = 3512;
    packets6[6].dfield[2] = 4660;
    packets6[6].dfield[3] = 1;
    packets6[6].dfield[4] = 1;
    packets6[6].dfield[5] = 1;
    packets6[6].dfield[6] = 1;
    packets6[6].dfield[7] = 1;
    cout << std::hex << 6 <<"                " << packets6[5].sfield[0] << ":" <<packets6[5].sfield[1] <<":" << packets6[5].sfield[2]<<":"<< packets6[5].sfield[3] <<"::                " << 
    packets6[5].dfield[0] << ":" << packets6[5].dfield[1] <<":" << packets6[5].dfield[2]<<":"<< packets6[5].dfield[3] << ":" << packets6[5].dfield[4] << ":" << packets6[5].dfield[6] 
    << ":" << packets6[5].dfield[7] << endl;
    //created a packet to test out for interface 0
    packets6[7].ver = 1;
    packets6[7].dfield[0] = 0;
    packets6[7].dfield[1] = 0;
    packets6[7].dfield[2] = 0;
    packets6[7].dfield[3] = 0;
    packets6[7].dfield[4] = 0;
    packets6[7].dfield[5] = 0;
    packets6[7].dfield[6] = 0;
    packets6[7].dfield[7] = 1;
    cout << std::hex << 7 <<"                " << packets6[5].sfield[0] << ":" <<packets6[5].sfield[1] <<":" << packets6[5].sfield[2]<<":"<< packets6[5].sfield[3] <<"::                " << 
    packets6[5].dfield[0] << ":" << packets6[5].dfield[1] <<":" << packets6[5].dfield[2]<<":"<< packets6[5].dfield[3] << ":" << packets6[5].dfield[4] << ":" << packets6[5].dfield[6] 
    << ":" << packets6[5].dfield[7] << endl;
    cout << endl << endl;
    //Print them out and display what interfaces they are heading
    for (int i = 0; i < 8;i++)
    {
        int j = 0;
        if(i < 5)
        {
            packets6[i].hop_limit -= 1;  //Update Hop Limit by decrementing it by 1; if hop goes below 0, drop it
        }
        if(packets6[i].hop_limit < 0)
        {
            cout << "Packet[" << i << "] was dropped due to expired Hop Limit!!" << endl;
        }
        else
        {
            cout << std::hex << "Packet["<< i <<"] is going from " << packets6[i].sfield[j] << ":" <<packets6[i].sfield[j+1] <<":" << packets6[i].sfield[j+2]<<":"<< packets6[i].sfield[j+3] <<":: to " << 
            packets6[i].dfield[j] << ":" << packets6[i].dfield[j+1] <<":" << packets6[i].dfield[j+2]<<":"<< packets6[i].dfield[j+3] << ":" << packets6[i].dfield[j+4] << ":" << packets6[i].dfield[j+4] << ":" << packets6[i].dfield[j+5]
            << ":" << packets6[i].dfield[j+6] << ":" << packets6[i].dfield[j+7] << " routed on interface " << longestIPv6Match(packets6[i], &itable) << endl; 
        }
    }
    return 0;
}