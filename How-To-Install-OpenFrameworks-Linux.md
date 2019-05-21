<h1> How to install OpenFrameworks on Linux - Quick Guide </h1>

<ol>
<li> Download the linux package from openframeworks.cc (alternatively fork the repo from github.com/openframeworks/openFrameworks)
<li> Unpack the package
<li> Go to /scripts/linux/ubuntu (if you're on ubuntu, or choose another, corresponding to your OS) and run "sudo ./install_dependencies.sh".
You may need to open the script and search for libgl1-mesa-dev-hwe-18.04. If it is there, change it to libgl1-mesa-dev (going by the library name libgl1-mesa-dev-hwe-18.04 it has something to do with Ubuntu 18.04, so if you're on that OS you should do the above).
<li> If you have forked the repo, go to the next step. 

If you're installing from package, go to your openframeworks folder, then libs/openFrameworks/utils, find ofConstants.h and open it in an editor of your choice.  Go to line 208 - 212 and inbetween 
<pre>#else // normal linux</pre> and <pre>#endif</pre> remove everything except for

`#include <GL/glew.h>`

<li> Go to your openframeworks folder, then /scripts/linux and run ./compileOF.sh. It may take even ~15 mins. You can run it with option -jX, where X is the chosen number of CPU cores to run it.

</ol>