build_mode = ARGUMENTS.get('mode', 'release')

if not (build_mode in ['debug', 'release']):
	print "Wrong build mode. Please see help."
	Exit(1)

release_flags = '-Os -s'
debug_flags = '-ggdb'

env = Environment(tools=['default', 'packaging'])
if build_mode == 'debug':
	env['CFLAGS'] = debug_flags
else:
	env['CFLAGS'] = release_flags
env.Program('bin/nvflash', Glob('*.c'))

conf = Configure(env)
if not conf.CheckLibWithHeader('usb-1.0', 'libusb-1.0/libusb.h', 'c'):
	print 'Did not find libusb-1.0.so or libusb.lib, exiting!'
	Exit(1)
else:
	env['LIBS']='usb-1.0'
env = conf.Finish()

env.Install('', 'bin/nvflash')
env.Package( 	NAME 		= 'nvflash',
		VERSION 	= '0.1',
		PACKAGEVERSION	= 0,
		PACKAGETYPE 	= 'tarbz2',
		LICENSE 	= 'gpl',
		SUMMARY 	= 'nvflash util',
		DESCRIPTION 	= 'Try to recreate nvflash util from Nvidia for flashing Tegra chips',
		X_RPM_GROUP	= 'Application/fu',
)

