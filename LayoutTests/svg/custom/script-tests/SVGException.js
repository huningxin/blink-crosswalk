description("Tests the properties of the exception thrown by rotateFromVector.")

var e;
try {
    var svgDoc = document.implementation.createDocument("http://www.w3.org/2000/svg", "svg", null);
    var matrix = svgDoc.documentElement.createSVGMatrix();
    matrix.rotateFromVector(0, 0)
    // raises a InvalidAccessError
} catch (err) {
    e = err;
}

shouldBeEqualToString("e.toString()", "InvalidAccessError: A parameter or an operation was not supported by the underlying object.");
shouldBeEqualToString("Object.prototype.toString.call(e)", "[object DOMException]");
shouldBeEqualToString("Object.prototype.toString.call(e.__proto__)", "[object DOMExceptionPrototype]");
shouldBeEqualToString("e.constructor.toString()", "function DOMException() { [native code] }");
shouldBe("e.constructor", "window.DOMException");

var successfullyParsed = true;
