#pragma once
#include <iostream>
struct ShaderCode {
	std::string buffer;
};
class ShaderCoder {
private:
	ShaderCode _buffer;
	void buffer(const char* add);
	void nl();
	void s();
public:
	ShaderCoder* version(const char* ver,const char* type);
	ShaderCoder* layout(const char* loc, const char* type, const char* name);
	ShaderCoder* methodStart(const char* returns, const char* methodName, std::initializer_list<const char*> parameters);
	ShaderCoder* methodEnd();

	ShaderCoder* line(const char* v0);

	ShaderCode build();
};
static ShaderCoder newShader() {
	return ShaderCoder();
}