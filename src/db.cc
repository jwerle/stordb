
#include <stdlib.h>
#include <string.h>
#include <v8.h>
#include <leveldb/db.h>

#include "stordb.h"
#include "db.h"

static char *stordb_err = NULL;

stordb_db_t *
stordb_db_new (stordb_t *sdb, const char *path) {
  stordb_db_t *db = (stordb_db_t *) malloc(sizeof(stordb_db_t));
  leveldb::DB *dbh;
  leveldb::Options opts;
  opts.create_if_missing = 1;
  leveldb::Status status = leveldb::DB::Open(opts, path, &dbh);
  if (!status.ok()) {
    stordb_err = (char *) status.ToString().c_str();
    free(db);
    return NULL;
  }
  db->dbh = dbh;
  db->path = strdup(path);
  return db;
}

void
stordb_db_bnew (const v8::FunctionCallbackInfo<v8::Value> &args) {
  // scope
  v8::HandleScope scope(args.GetIsolate());

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // db path
  v8::Handle<v8::Value> arg = args[0];
  v8::String::Utf8Value value(arg);
  char *path = *value;

  // db instance
  stordb_db_t *db = stordb_db_new(sdb, path);

  if (NULL == db) {
    if (stordb_err) {
      args.GetIsolate()->ThrowException(
          v8::String::NewFromUtf8(args.GetIsolate(), stordb_err));
      return args.GetReturnValue().Set(v8::Null(args.GetIsolate()));
    }
  }

  // v8 reference
  v8::Handle<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(args.GetIsolate());
  tpl->SetInternalFieldCount(1);
  v8::Local<v8::Object> obj = tpl->NewInstance();
  obj->SetInternalField(0, v8::External::New(args.GetIsolate(), db));

  // return to javascript
  args.GetReturnValue().Set(obj);
}

void
stordb_db_bget (const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Handle<v8::Value> arg;

  // scope
  v8::HandleScope scope(args.GetIsolate());

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // `stordb_db_t' v8 ref
  v8::Local<v8::Object> ref = args[0]->ToObject();
  v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(ref->GetInternalField(0));
  stordb_db_t *db = (stordb_db_t *) wrap->Value();

  // key
  arg = args[1];
  v8::String::Utf8Value vkey(arg);
  char *key = *vkey;

  // read
  std::string value;
  leveldb::Status status = db->dbh->Get(leveldb::ReadOptions(), key, &value);

  // handle error
  if (!status.ok()) {
      args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(
            args.GetIsolate(),
            status.ToString().c_str()));
      return args.GetReturnValue().Set(v8::Null(args.GetIsolate()));
  }

  // return
  args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), value.c_str()));
}

void
stordb_db_bput (const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Handle<v8::Value> arg;

  // scope
  v8::HandleScope scope(args.GetIsolate());

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // `stordb_db_t' v8 ref
  v8::Local<v8::Object> ref = args[0]->ToObject();
  v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(ref->GetInternalField(0));
  stordb_db_t *db = (stordb_db_t *) wrap->Value();

  // key
  arg = args[1];
  v8::String::Utf8Value vkey(arg);
  std::string key(*vkey);

  // value
  arg = args[2];
  v8::String::Utf8Value vvalue(arg);
  std::string value(*vvalue);

  // write
  leveldb::WriteOptions opts;
  leveldb::Status status = db->dbh->Put(opts, key, value);

  // handle error
  if (!status.ok()) {
      args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(
            args.GetIsolate(),
            status.ToString().c_str()));
      return args.GetReturnValue().Set(v8::False(args.GetIsolate()));
  }

  return args.GetReturnValue().Set(v8::True(args.GetIsolate()));
}

void
stordb_db_bdel (const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Handle<v8::Value> arg;

  // scope
  v8::HandleScope scope(args.GetIsolate());

  // current `stordb_t'
  stordb_t *sdb = stordb_get_current();

  // `stordb_db_t' v8 ref
  v8::Local<v8::Object> ref = args[0]->ToObject();
  v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(ref->GetInternalField(0));
  stordb_db_t *db = (stordb_db_t *) wrap->Value();

  // key
  arg = args[1];
  v8::String::Utf8Value vkey(arg);
  std::string key(*vkey);

  // delete
  leveldb::WriteOptions opts;
  leveldb::Status status = db->dbh->Delete(opts, key);

  // handle error
  if (!status.ok()) {
      args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(
            args.GetIsolate(),
            status.ToString().c_str()));
      return args.GetReturnValue().Set(v8::False(args.GetIsolate()));
  }

  return args.GetReturnValue().Set(v8::True(args.GetIsolate()));
}
