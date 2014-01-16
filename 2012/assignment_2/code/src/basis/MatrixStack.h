#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <list>
#include <base/Math.hpp>

class MatrixStack
{
public:
	MatrixStack         (void);

	void        clear   (void);
	FW::Mat4f   top     (void);
	void        push    (const FW::Mat4f& m);
	void        pop     (void);

private:

	std::list< FW::Mat4f > m_matrices;

};

#endif // MATRIX_STACK_H
