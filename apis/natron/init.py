path = "/home/pancho/Documents/GitHub/vinarender/apis/natron"
sys.path.insert(0, path)


def vina():
    import vina_to_natron

    reload(vina_to_natron)
    vina_to_natron.app = app1
    vina_to_natron.main()
