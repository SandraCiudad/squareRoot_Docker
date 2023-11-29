pipeline {
    agent any
    
    environment {
        ANALYSIS_DOCKER_IMAGE = 'debian_cppcheck:9.1'
        DOCKER_ARGS = '-u root' 
        ARTIFACT = 'libProject'
        ARTIFACT_TEST = 'PROJECT'
        PROJECT_NAME = "PROJECT"
    }

    stages {

        /*
        * The necessary prior configurarion is done 
        */

        stage('Build') {
            
            steps {
                sh "mkdir -p workspace"
                sh "mkdir -p reports"
                sh "mkdir -p doc"

                sh 'cmake -S . -B build'
                sh 'cmake --build build'
            }
        }
        

         /*
        * All tests are executed
        */

        stage('Tests') {

            agent any

            steps {
                
                script {
                
                    dir("${env.WORKSPACE}") {
                        sh 'make || true'
                    }

                }
            }
        } 

        /*
        * Code is analyzed using different tools: 
        *   - CPP Check 
        *   - CCCC
        *   - Doxygen
        *   - Valgrind
        */

        stage('Analysis') {
            
            agent {
                docker { 
                    image env.ANALYSIS_DOCKER_IMAGE         
                    args env.DOCKER_ARGS
                    reuseNode true
                }
            }

            steps {

                dir("${env.WORKSPACE}") {
                        
                    sh '''rm -rf reports/cccc'''
                    sh '''rm -rf reports/doxygen'''

                    sh '''cppcheck --enable=all --inconclusive --xml --xml-version=2 `find "." -name "*.c*" | grep -v ".cccc" | grep -v ".svn" | grep -v ".settings" | grep -v ".cproject"` 2> reports/project_cppcheck.xml'''

                    sh '''cccc --html_outfile=index.html `find "." -name "*.c*" | grep -v ".svn" | grep -v ".cccc" | grep -v ".settings" | grep -v ".cproject"`; mv .cccc reports/cccc; mv index.html reports/cccc'''

                    script {
                        try {
                            sh "/home/root/pmd/pmd-bin-6.47.0/bin/run.sh cpd --minimum-tokens 20 --language cpp --files /var/lib/jenkins/workspace/squareRoot_docker --format xml 1> reports/project_cpd.xml"
                        }
                        catch(e)
                        {
                            currentBuild.result = 'SUCCESS'
                        }
                    }
                        
                    sh '''mv /home/root/doxygen/doxyfile /home; cd /home; (cat doxyfile ; echo "PROJECT_NAME=PROJECT") | doxygen -; cd -; mv /home/doxygen reports'''

                    dir("${env.WORKSPACE}/build") {
                        sh 'cp executeTests /var/lib/jenkins/workspace/squareRoot_docker'
                        sh '''valgrind --tool=memcheck --leak-check=full --track-origins=yes --xml=yes --xml-file=../reports/project_valgrind.xml ./executeTests --gtest_filter=SquareRootTest.PositiveNos:SquareRootTest.NegativeNos'''
                    }
                

                    //---
                    dir("${env.WORKSPACE}/reports"){
                        sh 'ls'
                        sh 'cp project_cpd.xml /var/lib/jenkins/workspace/squareRoot_docker/reports'
                        sh 'cp project_cppcheck.xml /var/lib/jenkins/workspace/squareRoot_docker/reports'
                        sh 'cp project_valgrind.xml /var/lib/jenkins/workspace/squareRoot_docker/reports'
                    }

                    //junitReporter: { outputDir: '/reports', outputFile: 'project_cppcheck.xml', useBrowserName: false, xmlVersion: null }
                    /*dir("${env.WORKSPACE}.") 
                    {
                        dir('build'){
                            junit 'test_detail.xml'
                        }
                    }*/
                }
            }
        } 


        /*
        * All reports are published
        */
        
        stage('Reports') {
            
            agent {
                docker { 
                    image env.ANALYSIS_DOCKER_IMAGE         
                    args env.DOCKER_ARGS
                    reuseNode true
                }
            }

            steps {

                dir("${env.WORKSPACE}") {
                    
                    //publishCppcheck pattern: "reports/project_cppcheck.xml"
                    

                    //recordIssues(enabledForFailure: true, tool: cpd(pattern: "reports/project_cpd.xml"))

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


                    junit 'build/test_detail.xml'
                    
            
                    
                }
            }

        } 

    }
    post {         
        always {     

            // Publica el archivo XML como informe HTML             
            publishHTML(target: [                 
                allowMissing: false,                 
                alwaysLinkToLastBuild: false,                 
                keepAll: true,                 
                reportDir: 'reports',                 
                reportFiles: 'project_cpd.xml',                 
                reportName: 'CPP Report'             
            ])         
            

            publishHTML(target: [                 
                allowMissing: false,                 
                alwaysLinkToLastBuild: false,                 
                keepAll: true,                 
                reportDir: 'reports',                 
                reportFiles: 'project_cppcheck.xml',                 
                reportName: 'Cpp Check Report'             
            ])         

            publishHTML(target: [                 
                allowMissing: false,                 
                alwaysLinkToLastBuild: false,                 
                keepAll: true,                 
                reportDir: 'reports',                 
                reportFiles: 'project_valgrind.xml',                 
                reportName: 'Valgrind Report'             
            ])         
        } 
    }

}
