pipeline {
    agent any

    /*agent {
        node {
            label 'master'
        }
    }*/
    
    environment {
        //DOCKER_IMAGE = 'xsdk:j2.0'
        ANALYSIS_DOCKER_IMAGE = 'debian_cppcheck:9.1'
        // The following docker image is used for the tests
        //GMV_DOCKER_IMAGE = 'docker-tnb:20221003'
        DOCKER_ARGS = '-u root' //-u root es para dar permisos
        ARTIFACT = 'libProject'
        ARTIFACT_TEST = 'PROJECT'

        PROJECT_NAME = "PROJECT"
    }

    stages {

        stage('Prebuild') {
            
            steps {
                sh "mkdir -p workspace"
                sh "mkdir -p reports"
                sh "mkdir -p doc"

                sh 'cmake -S . -B build'
                sh 'cmake --build build'
                
                //sh "cp /home/jmenam/automated_compilation/compile.sh ."
                //sh "cp /home/jmenam/automated_compilation/compile_projects.sh ."
                //sh "cp /home/jmenam/automated_compilation/doxyconfig ."
                //sh "cp /home/jmenam/automated_compilation/enable_xsdk.sh ."

                //sh '''find -path "*/[Rr][Ee][Ll][Ee][Aa][Ss][Ee]_*/*" ! -name '*.sh' -delete'''
                //sh '''find -path "*/[Dd][Ee][Bb][Uu][Gg]_*/*" ! -name '*.sh' -delete'''
                
            }
        }
        
        stage('Build') {
            
            /*agent {
                docker { 
                    image env.DOCKER_IMAGE         
                    args env.DOCKER_ARGS
                    reuseNode true
                }
            }*/

            agent any

            steps {

                // -- Acciones dentro del container
                
                script {
                    
                    dir("${env.WORKSPACE}") {

                        /*// Compilation of all the needed projects
                        wrap([$class: 'Xvfb']) {
                            
                            sh '''./enable_xsdk.sh; ./compile.sh $PROJECT_SRC $TEST_SRC; cat compilation.txt;'''
                        }

                        //archieveArtifacts muestra en Jenkins archivos descargables (siempre y cuando no falle la ejecución)
                        archiveArtifacts artifacts: "${PROJECT_SWR}/Release_PC_Shared_Library/${ARTIFACT}.so", followSymlinks: false, onlyIfSuccessful: true
                        archiveArtifacts artifacts: "${PROJECT_SWR}/Debug_PC_Shared_Library/${ARTIFACT}.so", followSymlinks: false, onlyIfSuccessful: true
                        archiveArtifacts artifacts: "${PROJECT_SWR}/Release_PC_Static_Library/${ARTIFACT}.a", followSymlinks: false, onlyIfSuccessful: true
                        archiveArtifacts artifacts: "${PROJECT_SWR}/Debug_PC_Static_Library/${ARTIFACT}.a", followSymlinks: false, onlyIfSuccessful: true

                        archiveArtifacts artifacts: "${TEST_SWR}/Release_PC/${ARTIFACT_TEST}", followSymlinks: false, onlyIfSuccessful: true
                        archiveArtifacts artifacts: "${TEST_SWR}/Debug_PC/${ARTIFACT_TEST}", followSymlinks: false, onlyIfSuccessful: true
                        */
                        sh 'cmake -S . -B build'
                        sh 'cmake --build build'
                        sh 'make || true'

                    }
                }
            }
        } // Stage Build


        stage('Connect to Remote Host and Analysis Step') {
            
            /*agent {
                docker { 
                    image env.ANALYSIS_DOCKER_IMAGE         
                    args env.DOCKER_ARGS
                    reuseNode true        
                }
            }*/

            steps {
                script {
                    dir("${env.WORKSPACE}") {
                        //Connection and run docker
                        def remoteServer = '192.168.29.79'
                        def remoteUser = 'ci'
                        def remoteConnection = remoteUser + '@' + remoteServer
                        def docker_image = 'debian_cppcheck:9.1'
                        def remoteCommand = 'docker run -d --name deb_analysis ' + docker_image
                        def dockerCompose = 'docker-compose up -d'
                        //Analysis
                        def rm_cccc = "rm -rf reports/cccc" 
                        def rm_doxygen = "rm -rf reports/doxygen"
                        def cppcheck = "cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml"
                        def cccc = "cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; mv .cccc reports/cccc; mv index.html reports/cccc"
                        def cpd = "/home/root/pmd/pmd-bin-6.47.0/bin/run.sh cpd --minimum-tokens 20 --language cpp --files /var/lib/jenkins/workspace/project/$PROJECT_SRC --format xml 1> reports/project_cpd.xml"
                        def doxygen = "mv /home/root/doxygen/doxyfile /home; cd /home; (cat doxyfile ; echo "PROJECT_NAME=PROJECT") | doxygen -; cd -; mv /home/doxygen reports"
                        def valgrind = "valgrind --tool=memcheck --leak-check=full --track-origins=yes --xml=yes --xml-file=./reports/project_valgrind.xml ./executeTests --gtest_filter=SquareRootTest.PositiveNos:SquareRootTest.NegativeNos"

                        sshagent(credentials: ['ssh_dockerAgent_credentials']){
                            
                            sh """ ssh ${remoteConnection} docker-compose up -d """

                            def container_name = "deb_analysis9_1"

                            sh "echo removing old components..." 
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${rm_cccc}", returnStatus: true) 
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${rm_doxygen}", returnStatus: true)

                            sh "echo CPP CHECK CODE ANALYSIS" 
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${cppcheck}", returnStatus: true) 

                            sh "echo CCCC ANALYSIS" 
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${cccc}", returnStatus: true) 

                            sh "echo CPD ANALYSIS" 
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${cpd}", returnStatus: true) 

                            sh "echo GENERATINNG DOXYGEN DOCUMENTATION" 
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${doxygen}", returnStatus: true) 

                            sh "echo RUNNING VALGRIND" 
                            ls = "ls"
                            sh (script: "ssh ${remoteConnection} docker exec ${container_name} ${ls}", returnStatus: true) 

                        }   
                    }
                }
            }
        }

        /*stage('Analysis') {
            
            agent {
                docker { 
                    image env.ANALYSIS_DOCKER_IMAGE         
                    args env.DOCKER_ARGS
                    reuseNode true
                    
                }
            }

            steps {
                // -- Acciones dentro del container
                
                script {

                    def cppcheck_command = 'cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'

                    dir("${env.WORKSPACE}") {
                        
                        sh '''rm -rf reports/cccc'''
                        sh '''rm -rf reports/doxygen'''

                        // CPPCheck Code Analysis
                        sh '''cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'''

                        // CCCC Code Analysis
                        sh '''cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; mv .cccc reports/cccc; mv index.html reports/cccc'''

                        script {

                            try {
                                sh "/home/root/pmd/pmd-bin-6.47.0/bin/run.sh cpd --minimum-tokens 20 --language cpp --files /var/lib/jenkins/workspace/project/$PROJECT_SRC --format xml 1> reports/project_cpd.xml"
                            }
                            catch(e)
                            {
                                currentBuild.result = 'SUCCESS'
                            }
                        }
                        
                        // Generate Doxygen documentation                                  //modifica parámetros en el doxyfile (nombre)
                        sh '''ssh ci@192.168.29.79 mv /home/root/doxygen/doxyfile /home; cd /home; (cat doxyfile ; echo "PROJECT_NAME=PROJECT") | doxygen -; cd -; mv /home/doxygen reports'''

                        // Run Valgrind
                        dir("${env.WORKSPACE}/build") {
                            sh 'cp executeTests /var/lib/jenkins/workspace/squareRoot_docker'
                        }
                        sh '''ssh ci@192.168.29.79 valgrind --tool=memcheck --leak-check=full --track-origins=yes --xml=yes --xml-file=./reports/project_valgrind.xml ./executeTests --gtest_filter=SquareRootTest.PositiveNos:SquareRootTest.NegativeNos'''
                    }

                }
            }

        } // Stage Analysis*/

        stage('Tests') {
            
            agent {
                docker { 
                    image env.ANALYSIS_DOCKER_IMAGE         
                    args env.DOCKER_ARGS
                    reuseNode true
                }
            }
            
            steps {
                script{
                    def sshKeyFile = env.SSH_KEY
                    def command = './executeTests --gtest_output=xml'
                    
                    sh "sshpass -p AxoPmd4! ssh -i $sshKeyFile ci@192.168.29.79 ${cppcheck_command}"
                }
                /*dir("${env.WORKSPACE}") 
                {
                    sh './executeTests --gtest_output=xml'
                }*/
            }
        } // Stage Tests
        
        stage('Reports') {
            
            steps {

                dir("${env.WORKSPACE}") {

                    //publishCppcheck pattern: "reports/project_cppcheck.xml"

                    recordIssues(enabledForFailure: true, tool: cpd(pattern: "reports/project_cpd.xml"))

                    publishHTML([allowMissing: false, 
                                alwaysLinkToLastBuild: true, 
                                keepAll: true, 
                                reportDir: 'reports/cccc', 
                                reportFiles: 'index.html', 
                                reportName: 'CCCC Report', 
                                reportTitles: 'The CCCC report'])

                    publishHTML([allowMissing: false, 
                                alwaysLinkToLastBuild: true, 
                                keepAll: true, 
                                reportDir: 'reports/doxygen/html', 
                                reportFiles: 'index.html', 
                                reportName: 'Doxygen Report', 
                                reportTitles: 'Doxygen Report'])
                    
                    xunit([GoogleTest(excludesPattern: '', pattern: '*.xml', stopProcessingIfError: true)])
                }

                /*dir("${env.WORKSPACE}/reports") {
                    publishValgrind (
                        failBuildOnInvalidReports: true,
                        failBuildOnMissingReports: true,
                        failThresholdDefinitelyLost: '',
                        failThresholdInvalidReadWrite: '',
                        failThresholdTotal: '',
                        pattern: '*valgrind.xml',
                        publishResultsForAbortedBuilds: true,
                        publishResultsForFailedBuilds: true,
                        sourceSubstitutionPaths: '',
                        unstableThresholdDefinitelyLost: '',
                        unstableThresholdInvalidReadWrite: '',
                        unstableThresholdTotal: ''
                    )
                }*/

            }

        } // Stage Reports

    } // Stages
}
