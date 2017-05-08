
QMAKE_EXTRA_COMPILERS += copy_cfg_files
copy_cfg_files.name = COPY
copy_cfg_files.input = TEST_CFG_FILES
copy_cfg_files.output = ${QMAKE_FILE_BASE}.cfg
copy_cfg_files.CONFIG = no_link

win32 {
	copy_cfg_files.commands = copy /y ${QMAKE_FILE_IN} $$shell_path( $${OUT_PWD}/${QMAKE_FILE_BASE}.cfg )
}
else {
	copy_cfg_files.commands = cp ${QMAKE_FILE_IN} $${OUT_PWD}/${QMAKE_FILE_BASE}.cfg
}

PRE_TARGETDEPS += compiler_copy_cfg_files_make_all

INCLUDEPATH += ../../..
