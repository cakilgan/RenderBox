#include "ShaderCoder.h"
void ShaderCoder::buffer(const char* add){
	_buffer.buffer += add;
}
void ShaderCoder::nl(){
	buffer("\n");
}
void ShaderCoder::s() {
	buffer(" ");
}
ShaderCoder* ShaderCoder::version(const char* ver, const char* type) {
	buffer("#version ");
	buffer(ver);
	s();
	buffer(type);
	nl();
	return this;
}
ShaderCoder* ShaderCoder::layout(const char* loc, const char* type, const char* name) {
	buffer("layout (location = ");
	buffer(loc);
	buffer(") in ");
	buffer(type);
	s();
	buffer(name);
	buffer(";");
	nl();
	return this;
}
ShaderCoder* ShaderCoder::methodStart(const char* returns, const char* methodName, std::initializer_list<const char*> parameters) {
	buffer(returns);
	s();
	buffer(methodName);
	buffer("(");
	size_t i = 0;
	for (const auto& param : parameters) {
		buffer(param);
		if (i < parameters.size() - 1) buffer(", ");
		i++;
	}
	buffer(")");
	buffer("{");
	nl();
	return this;
}
ShaderCoder* ShaderCoder::methodEnd() {
	buffer("}");
	return this;
}

ShaderCoder* ShaderCoder::line(const char* v0) {
	buffer(v0);
	nl();
	return this;
}
ShaderCode ShaderCoder::build() {
		return _buffer;
}

