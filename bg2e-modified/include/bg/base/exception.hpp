/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _bg2e_exception_hpp_
#define _bg2e_exception_hpp_

#include <stdexcept>

#include <bg/export.hpp>

namespace bg {

#ifdef assert
#undef assert
#endif

BG2E_EXPORT extern void assert(bool,const std::exception &);

namespace base {

class InitException : public std::runtime_error {
public:
	InitException() :runtime_error("Initialization error") {}
	InitException(const std::string & msg) :runtime_error(msg) {}
	InitException(const char * msg) :runtime_error(msg) {}
};

class FileException : public std::runtime_error {
public:
	FileException() :runtime_error("File error") {}
	FileException(const std::string & msg) :runtime_error(msg) {}
	FileException(const char * msg) :runtime_error(msg) {}
};

class WriteFileException : public FileException {
public:
	WriteFileException() :FileException("Write file error") {}
	WriteFileException(const std::string & msg) :FileException(msg) {}
	WriteFileException(const char * msg) :FileException(msg) {}
};

class ReadFileException : public FileException {
public:
	ReadFileException() :FileException("Read file error") {}
	ReadFileException(const std::string & msg) :FileException(msg) {}
	ReadFileException(const char * msg) :FileException(msg) {}
};

class MalformedFileException : public ReadFileException {
public:
	MalformedFileException() :ReadFileException("Malformed file error") {}
	MalformedFileException(const std::string & msg) :ReadFileException(msg) {}
	MalformedFileException(const char * msg) :ReadFileException(msg) {}
};

class OverwriteFileException : public WriteFileException {
public:
	OverwriteFileException() :WriteFileException("Could not write file. A file already exists within the same path.") {}
	OverwriteFileException(const std::string & msg) :WriteFileException(msg) {}
	OverwriteFileException(const char * msg) :WriteFileException(msg) {}
};

class NoSuchFileException : public ReadFileException {
public:
	NoSuchFileException() :ReadFileException("No such file or directory") {}
	NoSuchFileException(const std::string & msg) :ReadFileException(msg) {}
	NoSuchFileException(const char * msg) :ReadFileException(msg) {}
};

class InvalidStateException : public std::runtime_error {
public:
	InvalidStateException() :std::runtime_error("Invalid state") {}
	InvalidStateException(const std::string & msg) :std::runtime_error(msg) {}
	InvalidStateException(const char * msg) :std::runtime_error(msg) {}
};

class InvalidParameterException : public std::runtime_error {
public:
	InvalidParameterException() :std::runtime_error("Invalid parameter") {}
	InvalidParameterException(const std::string & msg) :std::runtime_error(msg) {}
	InvalidParameterException(const char * msg) :std::runtime_error(msg) {}
};

class NullParameterException : public InvalidParameterException {
public:
	NullParameterException() :InvalidParameterException("Invalid null parameter") {}
	NullParameterException(const std::string & msg) :InvalidParameterException(msg) {}
	NullParameterException(const char * msg) :InvalidParameterException(msg) {}
};
	
class CompatibilityException : public std::runtime_error {
public:
	CompatibilityException() :std::runtime_error("System compatibility exception") {}
	CompatibilityException(const std::string & msg) :std::runtime_error(msg) {}
	CompatibilityException(const char * msg) :std::runtime_error(msg) {}
};

class PolygonException : public std::runtime_error {
public:
	PolygonException() :std::runtime_error("Polygon data exception") {}
	PolygonException(const std::string & msg) :std::runtime_error(msg) {}
	PolygonException(const char * msg) :std::runtime_error(msg) {}
};

class MalformedPolyListException : public PolygonException {
public:
	MalformedPolyListException() :PolygonException("Malformed polygon: the number of elements in polygon list does not match or is corrupted") {}
	MalformedPolyListException(const std::string & msg) :PolygonException(msg) {}
	MalformedPolyListException(const char * msg) :PolygonException(msg) {}
};

class ShaderException : public std::runtime_error {
public:
	ShaderException() :std::runtime_error("Shader error") {}
	ShaderException(const std::string & msg) :std::runtime_error(msg) {}
	ShaderException(const char * msg) :std::runtime_error(msg) {}
};

class ShaderCompileException : public ShaderException {
public:
	ShaderCompileException() :ShaderException("Shader compile error") {}
	ShaderCompileException(const std::string & msg) :ShaderException(msg) {}
	ShaderCompileException(const char * msg) :ShaderException(msg) {}
};

class ShaderLinkException : public ShaderException {
public:
	ShaderLinkException() :ShaderException("Shader link error") {}
	ShaderLinkException(const std::string & msg) :ShaderException(msg) {}
	ShaderLinkException(const char * msg) :ShaderException(msg) {}
};


class EngineException : public std::runtime_error {
public:
	EngineException() :std::runtime_error("Low level engine exception") {}
	EngineException(const std::string & msg) :std::runtime_error(msg) {}
	EngineException(const char * msg) :std::runtime_error(msg) {}
};

class DirectXEngineException : public EngineException {
public:
	DirectXEngineException() :EngineException("DirectX engine exception") {}
	DirectXEngineException(const std::string & msg) :EngineException(msg) {}
	DirectXEngineException(const char * msg) :EngineException(msg) {}
};

class OpenGLCoreEngineException : public EngineException {
public:
	OpenGLCoreEngineException() :EngineException("OpenGL Core engine exception") {}
	OpenGLCoreEngineException(const std::string & msg) :EngineException(msg) {}
	OpenGLCoreEngineException(const char * msg) :EngineException(msg) {}
};

class OpenGLES3EngineException : public EngineException {
public:
	OpenGLES3EngineException() :EngineException("OpenGL ES 3 engine exception") {}
	OpenGLES3EngineException(const std::string & msg) :EngineException(msg) {}
	OpenGLES3EngineException(const char * msg) :EngineException(msg) {}
};

class VulkanEngineException : public EngineException {
public:
	VulkanEngineException() : EngineException("Vulkan engine exception") {}
	VulkanEngineException(const std::string & msg) :EngineException(msg) {}
	VulkanEngineException(const char * msg) :EngineException(msg) {}
};

class JsonException : public FileException {
public:
	JsonException() :FileException("JSON exception.") {}
	JsonException(const std::string & msg) :FileException(msg) {}
	JsonException(const char * msg) :FileException(msg) {}
};

class JsonParseException : public JsonException {
public:
	JsonParseException() :JsonException("JSON parse exception.") {}
	JsonParseException(const std::string & msg) :JsonException(msg) {}
	JsonParseException(const char * msg) :JsonException(msg) {}
};

class PluginException : public std::runtime_error {
public:
	PluginException() :std::runtime_error("Plugin exception.") {}
	PluginException(const std::string & msg) :std::runtime_error(msg) {}
	PluginException(const char * msg) :std::runtime_error(msg) {}
};

class NoSuchPluginException : public PluginException {
public:
	NoSuchPluginException() :PluginException("No such plugin to load file") {}
	NoSuchPluginException(const std::string & msg) :PluginException(msg) {}
	NoSuchPluginException(const char * msg) :PluginException(msg) {}
};

class AudioException : public std::runtime_error {
public:
	AudioException() :runtime_error("Audio error") {}
	AudioException(const std::string & msg) :runtime_error(msg) {}
	AudioException(const char * msg) :runtime_error(msg) {}
};

class AudioCreateException : public AudioException {
public:
	AudioCreateException() :AudioException("Error creating audio resource") {}
	AudioCreateException(const std::string & msg) :AudioException(msg) {}
	AudioCreateException(const char * msg) :AudioException(msg) {}
};

class ScriptException : public std::runtime_error {
public:
	ScriptException() :runtime_error("Script exception") {}
	ScriptException(const std::string & msg) :runtime_error(msg) {}
	ScriptException(const char * msg) :runtime_error(msg) {}
};

class LuaException : public ScriptException {
public:
	LuaException() :ScriptException("Lua script exception") {}
	LuaException(const std::string & msg) :ScriptException(msg) {}
	LuaException(const char * msg) :ScriptException(msg) {}
};

class LuaTypeException : public ScriptException {
public:
	LuaTypeException() :ScriptException("Lua exception: unexpected type found in script.") {}
	LuaTypeException(const std::string & msg) :ScriptException(msg) {}
	LuaTypeException(const char * msg) :ScriptException(msg) {}
};

class LuaUndefinedSymbolException : public ScriptException {
public:
	LuaUndefinedSymbolException() :ScriptException("Lua exception: undefined symbol.") {}
	LuaUndefinedSymbolException(const std::string & msg) :ScriptException(msg) {}
	LuaUndefinedSymbolException (const char * msg) :ScriptException(msg) {}
};

class UniqueIdentifierException : public std::runtime_error {
public:
	UniqueIdentifierException() :std::runtime_error("Unique identifier exception.") {}
	UniqueIdentifierException(const std::string & msg) :std::runtime_error(msg) {}
	UniqueIdentifierException(const char * msg) :std::runtime_error(msg) {}
};

class InvalidUniqueIdentifierException : public UniqueIdentifierException {
public:
	InvalidUniqueIdentifierException() :UniqueIdentifierException("Trying to set an invalid unique identifier.") {}
	InvalidUniqueIdentifierException(const std::string & msg) :UniqueIdentifierException(msg) {}
	InvalidUniqueIdentifierException(const char * msg) :UniqueIdentifierException(msg) {}
};

class DuplicatedUniqueIdentifierException : public UniqueIdentifierException {
public:
	DuplicatedUniqueIdentifierException() :UniqueIdentifierException("The unique identifier is already used by other object.") {}
	DuplicatedUniqueIdentifierException(const std::string & msg) :UniqueIdentifierException(msg) {}
	DuplicatedUniqueIdentifierException(const char * msg) :UniqueIdentifierException(msg) {}
};

class FramebufferException : public std::runtime_error {
public:
	FramebufferException() :std::runtime_error("Framebuffer exception.") {}
	FramebufferException(const std::string & msg) :std::runtime_error(msg) {}
	FramebufferException(const char * msg) :std::runtime_error(msg) {}
};

class FramebufferCreationException : public FramebufferException {
public:
	FramebufferCreationException() :FramebufferException("Error creating framebuffer. Check the compatibility of the specified color planes.") {}
	FramebufferCreationException(const std::string & msg) :FramebufferException(msg) {}
	FramebufferCreationException(const char * msg) :FramebufferException(msg) {}
};

class NetException : public std::runtime_error {
public:
    NetException() :runtime_error("Network error.") {}
    NetException(const std::string & msg) :runtime_error(msg) {}
    NetException(const char * msg) :runtime_error(msg) {}
};

class ConnectionException : public NetException {
public:
    ConnectionException() :NetException("Connection error.") {}
    ConnectionException(const std::string & msg) :NetException(msg) {}
    ConnectionException(const char * msg) :NetException(msg) {}
};

class ConnectionClosedException : public ConnectionException {
public:
    ConnectionClosedException() :ConnectionException("The connection was closed in the remote host.") {}
    ConnectionClosedException(const std::string & msg) :ConnectionException(msg) {}
    ConnectionClosedException(const char * msg) :ConnectionException(msg) {}
};

class InputOutputNetException : public NetException {
public:
    InputOutputNetException() :NetException("Connection error.") {}
    InputOutputNetException(const std::string & msg) :NetException(msg) {}
    InputOutputNetException(const char * msg) :NetException(msg) {}
};

class ReadNetException : public InputOutputNetException {
public:
    ReadNetException() :InputOutputNetException("Could not read from socket.") {}
    ReadNetException(const std::string & msg) :InputOutputNetException(msg) {}
    ReadNetException(const char * msg) :InputOutputNetException(msg) {}
};

class WriteNetException : public InputOutputNetException {
public:
    WriteNetException() :InputOutputNetException("Could not write to socket.") {}
    WriteNetException(const std::string & msg) :InputOutputNetException(msg) {}
    WriteNetException(const char * msg) :InputOutputNetException(msg) {}
};

template <class ExceptionClass>
class Assert {
public:
    Assert(bool condition) {
        if (!condition) throw ExceptionClass();
    }
    Assert(bool condition, const std::string & msg) {
        if (!condition) throw ExceptionClass(msg);
    }
};

template <class ExceptionClass>
class ThrowWhen {
public:
    ThrowWhen(bool condition) {
        if (condition) throw ExceptionClass();
    }
    ThrowWhen(bool condition, const std::string & msg) {
        if (condition) throw ExceptionClass(msg);
    }
};

}
}

#endif
