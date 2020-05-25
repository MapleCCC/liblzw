#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include <string>
#include <vector>

void compress(const std::string& lzwfile, const std::vector<std::string>* header);
void decompress(const std::string& lzwfile);

#endif /* _INTERNAL_H_ */
