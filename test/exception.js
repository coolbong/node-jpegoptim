var assert = require('assert');
var jpegoptim = require('..');
//var mp3dat = require('../index.js');




//var test_file_1 = 'test/light_waves.jpg';
//var test_file_2 = 'test/swirl3768.jpg';
//var test_file_3 = 'test/swirl3777.jpg';


//TODO arguments test
//TODO optimize test
// 1. file not exist
// 2. file is broken
// 3. handle different file format



describe('parameter exceptions', function() {
	it('should throw as parameter error', function() {
		assert.throws(function() {jpegoptim.optimize('./doesnotexist.jpg')});
	});

	it('should throw as parameter error', function() {
		assert.throws(function() {jpegoptim.optimize('./doesnotexist.jpg', 'doesnotexist_output.jpg')});
	});

	it('should throw as third parameter must be number value', function() {
		assert.throws(function(){ jpegoptim.optimize('./doesnotexist.jpeg', './doesnotexist_output.jpg', '50') });
	});

	it('should throw as first parameter must be number value', function() {
		assert.throws(function(){ jpegoptim.optimize(100, './doesnotexist_output.jpg', '50') });
	});

	it('should throw as second parameter must be number value', function() {
		assert.throws(function(){ jpegoptim.optimize('./doesnotexist.jpg', 50, '50') });
	});

	it('should throw as parameter error', function() {
		assert.throws(function() {jpegoptim.lossless('./doesnotexist.jpg')});
	});

	it('should throw as second parameter must be number value', function() {
		assert.throws(function() {jpegoptim.lossless('./doesnotexist.jpg', 100)});
	});
});



describe('Exception', function() {
	it('should throw as jpeg image file does not exist', function() {
		assert.throws(function(){ jpegoptim.optimize('./doesnotexist.jpeg', './doesnotexist_output.jpg', 50) });
	});

	it('should throw as jpeg image file is broken', function() {
		assert.throws(function(){ jpegoptim.optimize('./test/img/broken.jpg', './test/img/broken_output.jpg', 50) });
	});

	it('should throw as jpeg image file is invalid (html file)', function() {
		assert.throws(function(){ jpegoptim.optimize('./test/img/invalidtest.html', './test/img/invalid_file_output.jpg', 50) });
	});

	it('should throw as jpeg image file does not exist', function() {
		assert.throws(function(){ jpegoptim.lossless('./doesnotexistlossless.jpeg', './doesnotexistlossless_output.jpg') });
	});

	it('should throw as jpeg image file is broken', function() {
		assert.throws(function(){ jpegoptim.optimize('./test/img/broken.jpg', './test/img/broken_output.jpg') });
	});

	it('should throw as jpeg image file is invalid(html file)', function() {
		assert.throws(function(){ jpegoptim.optimize('./test/img/invalidtest.html', './test/img/invalid_file_output.jpg') });
	});
});
