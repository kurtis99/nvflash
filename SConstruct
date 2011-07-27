env = Environment(tools=['default', 'packaging'])
env['paltform'] = 'posix'
env['LIBS'] = 'usb'
env.Program('bin/nvflash', ['nvflash.c'])

env.Install('/tmp/', 'nvflash')
env.Package( 	NAME 		= 'foo',
		VERSION 	= '1.2.3',
		PACKAGEVERSION	= 0,
		PACKAGETYPE 	= 'ipkg',
		LICENSE 	= 'gpl',
		SUMMARY 	= 'balalalalal',
		DESCRIPTION 	= 'this should be really really long',
		X_RPM_GROUP	= 'Application/fu',
		SOURCE_URL 	= 'http://foo.org/foo-1.2.3.tar.gz'
)

