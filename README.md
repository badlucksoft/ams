# ams

\mainpage

This project sketches out ideas for an Advanced Mail System that could help eliminate spam and enhance security. Make e-mail viable again.

This project is being implemented through the Eclipse IDE though only the sources are being added to the repository. Currently, this project is being implemented on Fedora Linux 24, and depends on libjansson, libuuid, and OpenSSL (libcrypto to be precise).

Changes
=======

11/26/2022
I realize this has basically been dead all these years; I apologize, my situation changed substantially in 2016 not long after I started this thing, and I've been busy with my day job ever since. I have also gone through a number of trials, not to mention a global pandemic, but this project was always on my mind and I do intend to get back to it. Maybe in 2023.

Any way, the changes I made tonight are not substantial, and barely worth mentioning. Changed some comparisons in Key.cpp, updated the Doxyfile from when documentation can/will be generated,and this README.md.


9/7/2016
Implementation of this project sketch is starting with the public key encryption and contact/identity management systems. This will be enhanced with networking features later. The current version of the Key class can generate a public-private key pair through use of OpenSSL functions. If and when LibreSSL becomes more widely supported (and even available in terms of development) this may be translated over to the more revised and LibreSSL.
