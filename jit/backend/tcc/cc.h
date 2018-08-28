struct JitCC_ {
  TCCState* s;
  m_str str;
  size_t len;
  struct Vector_ code;
  pthread_mutex_t mutex;
};
