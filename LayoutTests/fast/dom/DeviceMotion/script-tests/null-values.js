description('Tests using null values for some or all of the event properties.');

var mockEvent;
function setMockMotion(accelerationX, accelerationY, accelerationZ,
                       accelerationIncludingGravityX, accelerationIncludingGravityY, accelerationIncludingGravityZ,
                       rotationRateAlpha, rotationRateBeta, rotationRateGamma,
                       interval) {

    mockEvent = {accelerationX: accelerationX, accelerationY: accelerationY, accelerationZ: accelerationZ,
                 accelerationIncludingGravityX: accelerationIncludingGravityX, accelerationIncludingGravityY: accelerationIncludingGravityY, accelerationIncludingGravityZ: accelerationIncludingGravityZ,
                 rotationRateAlpha: rotationRateAlpha, rotationRateBeta: rotationRateBeta, rotationRateGamma: rotationRateGamma,
                 interval: interval};

    if (window.testRunner)
        testRunner.setMockDeviceMotion(null != mockEvent.accelerationX, null == mockEvent.accelerationX ? 0 : mockEvent.accelerationX,
                                       null != mockEvent.accelerationY, null == mockEvent.accelerationY ? 0 : mockEvent.accelerationY,
                                       null != mockEvent.accelerationZ, null == mockEvent.accelerationZ ? 0 : mockEvent.accelerationZ,
                                       null != mockEvent.accelerationIncludingGravityX, null == mockEvent.accelerationIncludingGravityX ? 0 : mockEvent.accelerationIncludingGravityX,
                                       null != mockEvent.accelerationIncludingGravityY, null == mockEvent.accelerationIncludingGravityY ? 0 : mockEvent.accelerationIncludingGravityY,
                                       null != mockEvent.accelerationIncludingGravityZ, null == mockEvent.accelerationIncludingGravityZ ? 0 : mockEvent.accelerationIncludingGravityZ,
                                       null != mockEvent.rotationRateAlpha, null == mockEvent.rotationRateAlpha ? 0 : mockEvent.rotationRateAlpha,
                                       null != mockEvent.rotationRateBeta, null == mockEvent.rotationRateBeta ? 0 : mockEvent.rotationRateBeta,
                                       null != mockEvent.rotationRateGamma, null == mockEvent.rotationRateGamma ? 0 : mockEvent.rotationRateGamma,
                                       interval);
    else
        debug('This test can not be run without the TestRunner');
}


var deviceMotionEvent;
function checkMotion(event) {
    deviceMotionEvent = event;
    shouldBe('deviceMotionEvent.acceleration.x', 'mockEvent.accelerationX');
    shouldBe('deviceMotionEvent.acceleration.y', 'mockEvent.accelerationY');
    shouldBe('deviceMotionEvent.acceleration.z', 'mockEvent.accelerationZ');

    shouldBe('deviceMotionEvent.accelerationIncludingGravity.x', 'mockEvent.accelerationIncludingGravityX');
    shouldBe('deviceMotionEvent.accelerationIncludingGravity.y', 'mockEvent.accelerationIncludingGravityY');
    shouldBe('deviceMotionEvent.accelerationIncludingGravity.z', 'mockEvent.accelerationIncludingGravityZ');

    shouldBe('deviceMotionEvent.rotationRate.alpha', 'mockEvent.rotationRateAlpha');
    shouldBe('deviceMotionEvent.rotationRate.beta', 'mockEvent.rotationRateBeta');
    shouldBe('deviceMotionEvent.rotationRate.gamma', 'mockEvent.rotationRateGamma');

    shouldBe('deviceMotionEvent.interval', 'mockEvent.interval');
}

function firstListener(event) {
    checkMotion(event);
    window.removeEventListener('devicemotion', firstListener);
    setTimeout(function(){initSecondListener();}, 0);
}

function initSecondListener() {
    setMockMotion(1, 2, 3,
              null, null, null,
              null, null, null,
              100);
    window.addEventListener('devicemotion', secondListener);
}

function secondListener(event) {
    checkMotion(event);
    window.removeEventListener('devicemotion', secondListener);
    setTimeout(function(){initThirdListener();}, 0);
}

function initThirdListener() {
    setMockMotion(null, null, null,
                  1, 2, 3,
                  null, null, null,
                  100);
    window.addEventListener('devicemotion', thirdListener);
}

function thirdListener(event) {
    checkMotion(event);
    window.removeEventListener('devicemotion', thirdListener);
    setTimeout(function(){initFourthListener();}, 0);
}

function initFourthListener() {
    setMockMotion(null, null, null,
                  null, null, null,
                  1, 2, 3,
                  100);
    window.addEventListener('devicemotion', fourthListener);
}

function fourthListener(event) {
    checkMotion(event);
    finishJSTest();
}

setMockMotion(null, null, null,
              null, null, null,
              null, null, null,
              0);
window.addEventListener('devicemotion', firstListener);

window.jsTestIsAsync = true;
