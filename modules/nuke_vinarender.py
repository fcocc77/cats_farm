from vinarender_util import jread, fread, fwrite
import os
import subprocess
import json
import nuke

vinarender_user = os.path.expanduser('~') + '/.vinarender'
vinarender_path = '/opt/vinarender'

def submit():
    try:
        selected_node = nuke.selectedNode()
    except:
        nuke.message("You must select a Write Node")
        return

    if check_project():
        if selected_node.Class() == 'Write':
            open_panel(selected_node)
        else:
            nuke.message("You must select a Write Node")


def get_absolute(filename):
    if not '..' in filename:
        return filename

    return os.path.abspath(nuke.script_directory() + '/' + filename)


def open_panel(write_node):

    first_frame = str(write_node.firstFrame())
    last_frame = str(write_node.lastFrame())

    project_file = nuke.root().knob('name').value()
    project_name = project_file.split("/")[-1].split(".")[0]

    p = nuke.Panel('VinaRender Submit')
    p.addSingleLineInput('Job Name', project_name)

    p.addSingleLineInput('Frame Range', first_frame + '-' + last_frame)
    p.addSingleLineInput('Task Size', '10')

    p.addEnumerationPulldown( 'Priority', "Very\\ High High Normal Low Very\\ Low")

    settings_from_manager = jread(vinarender_user + '/settings_from_manager.json')

    nuke_groups = ["NUKE", "nuke", "Nuke"]
    server_groups = []
    for group in settings_from_manager["groups"]:
        if not group in nuke_groups:
            server_groups.append(group)
        else:
            server_groups.insert(0, group)

    if server_groups:
        groups = ''
    else:
        groups = 'none'

    for server in server_groups:
        groups += ' ' + server

    p.addEnumerationPulldown('Server Group', groups)
    p.addEnumerationPulldown('Instances', "1 2 3 4")
    p.addBooleanCheckBox('Manually Start Job', False)

    p.addButton('Cancel')
    p.addButton('Send')

    if p.show():
        send(write_node, p)


def send(write_node, panel):

    job_name = str(panel.value("Job Name"))

    first_frame = int(panel.value("Frame Range").split("-")[0])
    last_frame = int(panel.value("Frame Range").split("-")[1])
    task_size = int(panel.value("Task Size"))
    priority_string = str(panel.value("Priority"))

    priority = 2

    if priority_string == 'Very High':
        priority = 0

    elif priority_string == 'High':
        priority = 1

    elif priority_string == 'Low':
        priority = 3

    elif priority_string == 'Very Low':
        priority = 4

    server_group = str(panel.value("Server Group"))
    instances = int(panel.value("Instances"))

    paused = panel.value("Manually Start Job")

    abs_filename = get_absolute(write_node["file"].value())
    render_node = write_node['name'].value()
    project_file = nuke.root().knob('name').value()

    basename = os.path.basename(abs_filename)
    dirname = os.path.dirname(abs_filename)

    ext = abs_filename.split(".")[-1]

    nuke.scriptSave()
    project_data = fread(project_file)

    if ext == 'mov':
        basename_no_ext = basename.split('.')[-2]
        new_filename = dirname + '/' + basename_no_ext + \
            '/' + basename_no_ext + '_###########.' + ext

        project_data = project_data.replace(abs_filename, '"' + new_filename + '"')


    new_project = get_available_project(project_file)
    fwrite(new_project, project_data)

    software_data = {
        'project': new_project,
        'filename': abs_filename,
        'render_node': render_node}

    submit_data = {
            'job_name': job_name,
            'paused': paused,
            'server_group': server_group,
            'priority': priority,
            'software': 'Nuke',
            'software_data': software_data,
            'system': 'Linux',
            'first_frame': first_frame,
            'last_frame': last_frame,
            'task_size': task_size,
            'instances': instances}

    submit_exe = vinarender_path + '/bin/submit'
    subprocess.Popen([submit_exe, json.dumps(submit_data)])

    nuke.message("Job Sended.")


def get_available_project(project_file):

    project_dir = os.path.dirname(project_file)
    project_name = os.path.basename(project_file).split('.')[0]

    available_project = project_dir + '/.' + project_name + 'render_01.nk'

    for number in range(1, 1000):
        exist_file = os.path.isfile(available_project)

        if number < 10:
            number = '0' + str(number)
        else:
            number = str(number)

        if exist_file:
            available_project = project_dir + '/.' + \
                project_name + '_render_' + number + '.nk'
        else:
            break

    return available_project


def check_project():

    settings_from_manager = jread(vinarender_user + '/settings_from_manager.json')
    path_list = settings_from_manager['paths']['system']

    rutas = []
    for r in path_list:
        if os.path.isdir(r):
            rutas.append(r)
    if not rutas:
        rutas.append("#none#")


    failed_nodes = ""
    desconected_nodes = ""

    checked = True

    nuke.selectConnectedNodes()

    for node in nuke.selectedNodes():
        name = node.name()
        try:
            file = node["file"].getText()
        except:
            file = None

        if file:
            dirname = os.path.dirname(file)
            relative = '..'

            if relative in file:
                None
            elif not os.path.isdir(dirname):
                desconected_nodes = desconected_nodes + name + '=' + file + '\n'
                checked = False
            else:
                f = False
                for ruta in rutas:
                    if ruta in file:
                        f = True
                if not f:
                    checked = False
                    failed_nodes = failed_nodes+name+"="+file+"\n"

    for n in nuke.allNodes():
        n.knob("selected").setValue(False)

    if not checked:
        if failed_nodes:
            line1 = "These files are on your PC:"
        else:
            line1 = ""

        if desconected_nodes:
            line2 = "These files are disconnected:"
        else:
            line2 = ""

        message = line1 + '\n\n' + failed_nodes + \
            '\n\n' + line2 + '\n\n' + desconected_nodes

        nuke.message(message)

    return checked
