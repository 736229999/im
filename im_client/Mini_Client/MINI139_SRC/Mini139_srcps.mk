
Mini139_srcps.dll: dlldata.obj Mini139_src_p.obj Mini139_src_i.obj
	link /dll /out:Mini139_srcps.dll /def:Mini139_srcps.def /entry:DllMain dlldata.obj Mini139_src_p.obj Mini139_src_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Mini139_srcps.dll
	@del Mini139_srcps.lib
	@del Mini139_srcps.exp
	@del dlldata.obj
	@del Mini139_src_p.obj
	@del Mini139_src_i.obj

Mini139_srcps.dll: dlldata.obj Mini139_src_p.obj Mini139_src_i.obj
	link /dll /out:Mini139_srcps.dll /def:Mini139_srcps.def /entry:DllMain dlldata.obj Mini139_src_p.obj Mini139_src_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Mini139_srcps.dll
	@del Mini139_srcps.lib
	@del Mini139_srcps.exp
	@del dlldata.obj
	@del Mini139_src_p.obj
	@del Mini139_src_i.obj
