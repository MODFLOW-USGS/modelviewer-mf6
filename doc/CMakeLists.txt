# install (doc)
install(
  FILES
    ofr02-106.pdf
  DESTINATION
    $<$<PLATFORM_ID:Linux>:${CMAKE_INSTALL_DOCDIR}>$<$<PLATFORM_ID:Windows>:doc>
)
