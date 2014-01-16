#include "MatrixStack.h"
#include "extra.h"

using namespace FW;

MatrixStack::MatrixStack()
{
	Mat4f init;
	init.setIdentity();
	m_matrices.push_front(init);
}

void MatrixStack::clear()
{
	m_matrices.clear();
	Mat4f init;
	init.setIdentity();
	m_matrices.push_front(init);
}

FW::Mat4f MatrixStack::top()
{
	return m_matrices.front();
}

void MatrixStack::push( const FW::Mat4f& m )
{
	m_matrices.push_front(m_matrices.front()*m);
}

void MatrixStack::pop()
{
	m_matrices.pop_front();
}
