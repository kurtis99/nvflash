env = Environment(tools=['default', 'packaging'])
env['paltform'] = 'posix'
env['LIBS'] = 'usb-1.0'
env.Program('bin/nvflash', ['nvflash.c'])

env.Install('', 'bin/nvflash')
env.Package( 	NAME 		= 'nvflash',
		VERSION 	= '0.1',
		PACKAGEVERSION	= 0,
		PACKAGETYPE 	= 'tarbz2',
		LICENSE 	= 'gpl',
		SUMMARY 	= 'nvflash util',
		DESCRIPTION 	= 'Try to recreate nvflash util from Nvidia for flashing Tegra chips',
		X_RPM_GROUP	= 'Application/fu',
#		SOURCE_URL 	= 'http://foo.org/foo-1.2.3.tar.gz'
)

