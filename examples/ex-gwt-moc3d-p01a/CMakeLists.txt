install(
  FILES 
    ex-gwt-moc3d-p01a-conc.mvmf6
    ex-gwt-moc3d-p01a-head.mvmf6
  DESTINATION
    $<$<PLATFORM_ID:Linux>:${CMAKE_INSTALL_DOCDIR}/>examples/ex-gwt-moc3d-p01a
)

add_subdirectory(mf6gwf)
add_subdirectory(mf6gwt)
