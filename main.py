import numpy as np 
import matplotlib.pyplot as plt
from tensorflow import keras

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation

import numpy as np
m = 2 # slope
b = 1 # intersect

batch=10

np.random.seed(0)
x_train = np.random.uniform(-1, 1, 500)
x_valid = np.random.uniform(-1, 1, 50)
x_valid.sort()
y_target = m * x_valid + b # ideal (target) linear function

sigma = 0.2# noise standard deviation, for the moment it is absent
y_train = np.random.normal(m * x_train + b, sigma) # actual measures from which we want to guess regression parameters
y_valid = np.random.normal(m * x_valid + b, sigma)

model = keras.Sequential()
model.add(Dense(1, input_shape=(1,)))

# compile the model choosing optimizer, loss and metrics objects
model.compile(optimizer='sgd', loss='mse', metrics=['mse'])
model.summary()


history = model.fit(x=x_train, y=y_train, 
          batch_size=batch, epochs=100,
          shuffle=True, # a good idea is to shuffle input before at each epoch
          validation_data=(x_valid, y_valid),verbose=False)


for l in model.layers:

    w=model.get_weights()

    print(w[0],w[1])


# evaluate model



score = model.evaluate(x_valid, y_valid, batch_size=batch, verbose=False)

# print performance
print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])


# evaluate model with the exact curve
score = model.evaluate(x_valid, y_target, batch_size=batch, verbose=False)

# print performance
print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])

# look into training history
# Plot training & validation loss values
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.show()