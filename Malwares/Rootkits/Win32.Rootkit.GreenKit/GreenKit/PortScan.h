#pragma once
#include <vector>

void PortScan();
std::vector<int> parseRangePort(int min, int max);
void sendFile(std::vector<int> open_ports, std::string filename);



