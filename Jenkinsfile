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

                        sh 'make || true'

                    }
                }
            }
        } // Stage Build


        stage('Connect to Remote Host and Run Docker') {
            steps {
                script {
                    def password = 'AxoPmd4!'
                    def remoteHost = '192.168.29.79'
                    def sshUser = 'ci'
                    //def sshKeyCredentialId = 'docker_SSH_conection'
                    def dockerImage = 'debian_cppcheck:9.1'
                    def commandToRun = 'docker run -d ' + dockerImage 
                    //def cppcheck_command = 'cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'
                    def cccc = 'cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; mv .cccc reports/cccc; mv index.html reports/cccc' 
                    def tests = './executeTests --gtest_output=xml'
                    def publish_cccc = publishHTML([allowMissing: false, 
                                alwaysLinkToLastBuild: true, 
                                keepAll: true, 
                                reportDir: 'reports/cccc', 
                                reportFiles: 'index.html', 
                                reportName: 'CCCC Report', 
                                reportTitles: 'The CCCC report'])

                    withCredentials([sshUserPrivateKey(credentialsId: 'docker_SSH_conection', keyFileVariable: 'SSH_KEY')]) {
                        //sh "sshpass -p ${password} ssh ${sshUser}@${remoteHost} '${commandToRun}' && ${cccc} && ${tests} && ${publish_cccc}"
                        sh 'sshpass -p AxoPmd4! ssh ci@192.168.29.79 docker run -d debian_cppcheck:9.1 && cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; cat index.html'
                    }

                    


                    /*script {
                        def sshAgentStep = sshagent(['docker_SSH_conection'])
                        try {
                            // Within the SSH agent context, run your SSH commands
                            sh "sshpass -p ${password} ssh ${sshUser}@${remoteHost} '${commandToRun}' ${cppcheck_command}"
                        } finally {
                            // Stop the SSH agent and capture the exit status
                            def sshAgentExitStatus = sshAgentStep.stop()
                            
                            if (sshAgentExitStatus != 0) {
                                currentBuild.result = 'FAILURE'
                                error("SSH agent failed with exit status: $sshAgentExitStatus")
                            }
                        }
                    }*/

                    /*def isAgentRunning = sh(script: 'ps aux | grep ssh-agent | grep -v grep', returnStatus: true) == 0

                    if (!isAgentRunning) {
                        echo 'SSH agent is not running. Starting SSH agent...'
                        sh 'eval `ssh-agent`'
                        sshagent(['docker_SSH_conection']) {
                            sh "sshpass -p ${password} ssh ${sshUser}@${remoteHost} '${commandToRun}' ${cppcheck_command}"
                        }
                    } else {
                        sshagent(['docker_SSH_conection']) {
                            sh "sshpass -p ${password} ssh ${sshUser}@${remoteHost} '${commandToRun}' ${cppcheck_command}"
                        }
                    }*/
                


                    /*sshagent(['docker_SSH_conection']) {
                        
                        sh "sshpass -p ${password} ssh ${sshUser}@${remoteHost} '${commandToRun}' ${cppcheck_command}"

                        script {
                            dir("${env.WORKSPACE}") {
                                
                                sh '''rm -rf reports/cccc'''
                                sh '''rm -rf reports/doxygen'''

                                // CPPCheck Code Analysis
                                sh '''ssh ci@192.168.29.79 cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'''

                                // CCCC Code Analysis
                                sh '''ssh ci@192.168.29.79 cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; mv .cccc reports/cccc; mv index.html reports/cccc'''

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




                        //sh "sshpass -p ${password} ssh ${sshUser}@${remoteHost} ls"

                    }*/
                    /*sshScript remote: remoteHost, user: sshUser, credentialsId: sshKeyCredentialId, script: """
                        ssh -o StrictHostKeyChecking=no \$user@\$remoteHost 'docker run \$dockerImage'
                    """*/
                }
            }
        }

        stage('Analysis') {
            
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

                    def sshKeyFile = env.SSH_KEY
                    def cppcheck_command = 'cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'
                    
                    sh "sshpass -p AxoPmd4! ssh -i $sshKeyFile ci@192.168.29.79 ${cppcheck_command}"

                    /*dir("${env.WORKSPACE}") {
                        
                        sh '''rm -rf reports/cccc'''
                        sh '''rm -rf reports/doxygen'''

                        // CPPCheck Code Analysis
                        sh '''ssh ci@192.168.29.79 cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'''

                        // CCCC Code Analysis
                        sh '''ssh ci@192.168.29.79 cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; mv .cccc reports/cccc; mv index.html reports/cccc'''

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
                    }*/

                }
            }

        } // Stage Analysis

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
