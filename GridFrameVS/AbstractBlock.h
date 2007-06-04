#ifndef __ABSTRACT_BLOCK_H__
#define __ABSTRACT_BLOCK_H__

class AbstractBlock
{
protected:
	bool moveable;
	bool solid;

public:
	virtual void render() = 0;
	virtual void print(int i) = 0;

	bool isMoveable()
	{
		return this->moveable;
	}
	bool isSolid()
	{
		return this->solid;
	}
};

#endif // __ABSTRACT_BLOCK_H__

