
var global = this.global = this;

module env {
  export function get (key) {
    return global.ENV[key];
  }

  export function set (key, value) {
    global.ENV[key] = value;
    return this;
  }
}

