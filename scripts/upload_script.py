Import("env", "projenv")

def after_build(source, target, env):
  print("######### after build #############")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", after_build)