env = Environment(tools=['default', 'packaging'])
env['paltform'] = 'posix'
env['LIBS'] = 'usb-1.0'
env.Program('bin/nvflash', ['nvflash.c'])

env.Install('', 'bin/nvflash')
env.Package( 	NAME 		= 'foo',
		VERSION 	= '1.2.3',
		PACKAGEVERSION	= 0,
		PACKAGETYPE 	= 'tarbz2',
		LICENSE 	= 'gpl',
		SUMMARY 	= 'balalalalal',
		DESCRIPTION 	= 'this should be really really long',
		X_RPM_GROUP	= 'Application/fu',
		SOURCE_URL 	= 'http://foo.org/foo-1.2.3.tar.gz'
)

