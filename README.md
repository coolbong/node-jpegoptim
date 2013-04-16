# node-jpegoptim
      
Bindings to [jpegoptim](http://www.kokkonen.net/tjko/projects.html), [jpeglib](http://libjpeg.sourceforge.net/) for jpeg image optimize


## Example

	var jpegoptim = require('jpegoptim');
	var quality = 50; // 1~100

	jpegoptim.optimize('./img/input.jpg', './img/output.jpg', quality);


## Installation

You can install the latest tag via npm:

	npm install git://github.com/coolbong/node-jpegoptim.git


