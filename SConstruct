import os
import sys

def main():
	uname = os.uname()
	if uname[0] == 'Linux':
		operatingSystem = "linux"
	elif uname[0] == 'Darwin':
		operatingSystem = 'darwin'
	else:
		print "Unknown operating system: " + uname[0]
		sys.exit(1)

	baseEnv = initBaseEnv(operatingSystem)
	build(baseEnv)

def initBaseEnv(operatingSystem):
	baseEnv = Environment()
	conf = Configure(baseEnv)
	baseEnv = conf.Finish()

	# Normally, we'll have manually compiled stuff like Boost in
	# /usr/local on OS X.
	if operatingSystem == 'darwin':
		baseEnv.Append(CPPPATH = ['/usr/local/include'])

	# Local project paths and flags.
	baseEnv.Append(CPPPATH = ['#/'], LIBPATH = '#/build/lib',
		CXXFLAGS = ['-pedantic', '-Wall'])

	return baseEnv

def build(baseEnv):
	buildEnv = baseEnv.Copy()
	Export('buildEnv')

	# Define build directories.
	BuildDir('build/lib', 'lib')
	BuildDir('build/util', 'util')

	# Build
	buildEnv.SConscript([
		'build/lib/SConscript',
		'build/util/SConscript',
	])

main()
