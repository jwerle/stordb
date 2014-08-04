
module.exports = function(fn){
  var orig = Error.prepareStackTrace;
  Error.prepareStackTrace = function(_, stack){ return stack; };
  var err = new Error;
  Error.captureStackTrace(err, fn);
  var stack = err.stack;
  Error.prepareStackTrace = orig;
  return stack;
};
