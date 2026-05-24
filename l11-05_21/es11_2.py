import numpy as np 
import matplotlib.pyplot as plt
from tensorflow import keras

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation,Input

import numpy as np


def f(x):
    return 4-3*x-2*x**2+3*x**3




N_batch=20
N_epoch=50
N_neurons=10

np.random.seed(0)
x_train = np.random.uniform(-1, 1, 5000)
x_valid = np.random.uniform(-1, 1, 500)
x_valid.sort()
sigma = 0.2# noise standard deviation, for the moment it is absent
y_train = np.random.normal(f(x_train), sigma) # actual measures from which we want to guess regression parameters
y_valid = np.random.normal(f(x_valid), sigma)


model = keras.Sequential()
model.add(Input(1,))
model.add(Dense(5*N_neurons,activation='elu'))
model.add(Dense(4*N_neurons,activation='elu'))
model.add(Dense(3*N_neurons,activation='elu'))
model.add(Dense(2*N_neurons,activation='elu'))
model.add(Dense(N_neurons,activation='elu'))
model.add(Dense(1))

model.compile(optimizer='sgd', loss='mse', metrics=['mse'])
model.summary()

history = model.fit(x=x_train, y=y_train, 
          batch_size=N_batch, epochs=N_epoch, 
          validation_data=(x_valid, y_valid))


score = model.evaluate(x_valid, y_valid, batch_size=N_batch, verbose=1)
print('Test loss:', score[0])
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.show()



x_test=np.linspace(-1,1,200)
print(x_test.shape)
y_predicted = model.predict(x_test, batch_size=200)

plt.scatter(x_train,y_train,marker=".",label="Data")


plt.plot(x_test,f(x_test),color="orange",label="target")
plt.scatter(x_test,y_predicted.T,color="red",label="Predicted")


plt.legend()
plt.show()
