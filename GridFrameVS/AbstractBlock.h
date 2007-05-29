#ifndef __ABSTRACT_BLOCK_H__
#define __ABSTRACT_BLOCK_H__

class AbstractBlock
{
public:
	virtual void render() = 0;
	virtual void print(int i) = 0;
};

#endif // __ABSTRACT_BLOCK_H__

