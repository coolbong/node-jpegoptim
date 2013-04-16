var jpegoptim = require('./lib/jpegoptim');




jpegoptim.optimize('./test/img/light_waves.jpg', './test/img/light_waves_50.jpg', 50);
jpegoptim.optimize('./test/img/light_waves.jpg', './test/img/light_waves_70.jpg', 70);
jpegoptim.optimize('./test/img/light_waves.jpg', './test/img/light_waves_80.jpg', 80);



jpegoptim.lossless('./test/img/light_waves.jpg', './test/img/light_waves_lossless.jpg');
